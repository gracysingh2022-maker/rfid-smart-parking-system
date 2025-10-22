-- Donor table (hostel mess / block)
CREATE TABLE Donor (
    donor_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100),
    location VARCHAR(200),
    contact VARCHAR(20)
);

-- Meal batch table
CREATE TABLE MealBatch (
    batch_id INT AUTO_INCREMENT PRIMARY KEY,
    donor_id INT,
    quantity INT,                    -- number of meal packets
    cook_time DATETIME,
    expiry_time DATETIME,
    pickup_start DATETIME,
    pickup_end DATETIME,
    status ENUM('AVAILABLE','ASSIGNED','PICKED','DELIVERED','EXPIRED') DEFAULT 'AVAILABLE',
    FOREIGN KEY (donor_id) REFERENCES Donor(donor_id)
);

-- Recipient table (NGO / needy)
CREATE TABLE Recipient (
    recipient_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100),
    location VARCHAR(200),
    contact VARCHAR(20),
    capacity INT,                    -- max packets they can receive
    urgency INT DEFAULT 1            -- 1 low .. 10 high
);

-- Volunteer table
CREATE TABLE Volunteer (
    volunteer_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100),
    contact VARCHAR(20),
    location VARCHAR(200),
    is_available BOOLEAN DEFAULT TRUE
);

-- Assignments table (who picks which batch and delivers to which recipient)
CREATE TABLE Assignment (
    assignment_id INT AUTO_INCREMENT PRIMARY KEY,
    batch_id INT,
    volunteer_id INT,
    recipient_id INT,
    assign_time DATETIME DEFAULT CURRENT_TIMESTAMP,
    status ENUM('PENDING','PICKED','DELIVERED','CANCELLED') DEFAULT 'PENDING',
    FOREIGN KEY (batch_id) REFERENCES MealBatch(batch_id),
    FOREIGN KEY (volunteer_id) REFERENCES Volunteer(volunteer_id),
    FOREIGN KEY (recipient_id) REFERENCES Recipient(recipient_id)
);
