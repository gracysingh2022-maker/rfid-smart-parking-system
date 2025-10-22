#include <bits/stdc++.h>
using namespace std;

/*
 Simulation:
 - MealBatch arrives from Donor
 - Recipients stored with urgency and capacity
 - Volunteers available
 - Greedy matching using priority queues
*/

// ----- Structures -----
struct Recipient {
    int id;
    string name;
    int capacity;   // how many packets can receive
    int urgency;    // 1..10
    double distance; // distance from donor (lower is nearer)
};

struct Volunteer {
    int id;
    string name;
    double distance; // distance to donor (lower better)
    bool available;
};

// For max-heap of recipients: higher urgency first, then nearer distance
struct RecipientCmp {
    bool operator()(const Recipient &a, const Recipient &b) const {
        if (a.urgency != b.urgency) return a.urgency < b.urgency; // max-heap by urgency
        return a.distance > b.distance; // nearer preferred
    }
};

// For min-heap of volunteers: nearer volunteers first
struct VolunteerCmp {
    bool operator()(const Volunteer &a, const Volunteer &b) const {
        return a.distance > b.distance; // min-heap by distance
    }
};

// Simple in-memory DB simulators
unordered_map<int, Recipient> recipientsDB;
unordered_map<int, Volunteer> volunteersDB;

struct MealBatch {
    int batch_id;
    int donor_id;
    int quantity;
    string donor_loc;
    time_t cook_time;
    bool assigned;
};

struct Assignment {
    int batch_id;
    int volunteer_id;
    int recipient_id;
    int quantity_assigned;
};

// ----- Matching Function -----
vector<Assignment> matchBatchToRecipients(MealBatch &batch) {
    vector<Assignment> assignments;
    // Build max-heap of recipients with capacity > 0
    priority_queue<Recipient, vector<Recipient>, RecipientCmp> recHeap;
    for (auto &p : recipientsDB) {
        Recipient r = p.second;
        if (r.capacity > 0) {
            // For demo, compute distance as absolute difference between donor_id and recipient_id (stub),
            // but we assume Recipient.distance already set relative to donor.
            recHeap.push(r);
        }
    }

    // Build min-heap of available volunteers
    priority_queue<Volunteer, vector<Volunteer>, VolunteerCmp> volHeap;
    for (auto &p : volunteersDB) {
        Volunteer v = p.second;
        if (v.available) volHeap.push(v);
    }

    int remaining = batch.quantity;
    while (remaining > 0 && !recHeap.empty()) {
        Recipient topRec = recHeap.top(); recHeap.pop();
        int give = min(remaining, topRec.capacity);

        // Get a volunteer
        if (volHeap.empty()) {
            cout << "No volunteers available right now. Batch will be pending for remaining " << remaining << " packets.\n";
            break;
        }
        Volunteer vol = volHeap.top(); volHeap.pop();
        // Create assignment
        Assignment asg;
        asg.batch_id = batch.batch_id;
        asg.volunteer_id = vol.id;
        asg.recipient_id = topRec.id;
        asg.quantity_assigned = give;
        assignments.push_back(asg);

        // Update states
        remaining -= give;
        recipientsDB[topRec.id].capacity -= give;
        // Volunteer becomes unavailable for this simple demo (assume 1 trip)
        volunteersDB[vol.id].available = false;

        cout << "Assigned " << give << " packets of batch " << batch.batch_id
             << " to recipient " << topRec.id << " via volunteer " << vol.id << "\n";

        // If recipient still has capacity, push back with updated capacity
        if (recipientsDB[topRec.id].capacity > 0) {
            Recipient updated = recipientsDB[topRec.id];
            recHeap.push(updated);
        }
    }

    if (remaining == 0) {
        batch.assigned = true;
        cout << "Batch " << batch.batch_id << " fully assigned.\n";
    } else {
        batch.quantity = remaining;
        cout << "Batch " << batch.batch_id << " partially assigned. Remaining: " << remaining << "\n";
    }

    return assignments;
}

// ----- Demo Data & Main -----
int main() {
    // Seed recipients (id, name, capacity, urgency, distance)
    recipientsDB[1] = {1, "OldAgeHome", 10, 8, 1.2};
    recipientsDB[2] = {2, "Orphanage", 8, 9, 0.8};
    recipientsDB[3] = {3, "LocalNGO", 15, 6, 2.0};
    recipientsDB[4] = {4, "StreetVendors", 5, 7, 0.5};

    // Volunteers (id, name, distance to donor, available)
    volunteersDB[101] = {101, "VolunteerA", 0.5, true};
    volunteersDB[102] = {102, "VolunteerB", 1.0, true};
    volunteersDB[103] = {103, "VolunteerC", 2.5, true};

    // Incoming Meal Batch from hostel mess
    MealBatch batch1;
    batch1.batch_id = 1001;
    batch1.donor_id = 501; // hostel id
    batch1.quantity = 20;
    batch1.donor_loc = "HostelCanteen";
    batch1.assigned = false;

    cout << "Processing batch " << batch1.batch_id << " with quantity " << batch1.quantity << "\n\n";
    vector<Assignment> result = matchBatchToRecipients(batch1);

    cout << "\nAssignments created: " << result.size() << "\n";
    for (auto &a : result) {
        cout << "Batch " << a.batch_id << " -> Volunteer " << a.volunteer_id
             << " -> Recipient " << a.recipient_id << " : " << a.quantity_assigned << " packets\n";
    }

    cout << "\nFinal recipient capacities:\n";
    for (auto &p : recipientsDB) {
        cout << "Recipient " << p.first << " remaining capacity: " << p.second.capacity << "\n";
    }

    return 0;
}
