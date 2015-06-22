CREATE TABLE patients (
  id INTEGER PRIMARY KEY NOT NULL,
  name TEXT NOT NULL,
  room TEXT,
  device_id UNIQUE DEFAULT NULL
);

CREATE TABLE measurements_types (
  id INTEGER PRIMARY KEY NOT NULL,
  name TEXT NOT NULL,
  unit TEXT,
  token TEXT UNIQUE NOT NULL
);

CREATE TABLE measurements (
  id INTEGER PRIMARY KEY NOT NULL,
  datetime DATETIME NOT NULL DEFAULT (DATETIME('now')),
  patient_id INTEGER,
  measurement_type INTEGER,
  value REAL NOT NULL,
  FOREIGN KEY(patient_id) REFERENCES patients(id),
  FOREIGN KEY(measurement_type) REFERENCES measurements_types(id)
);

CREATE TABLE patients_limits (
  patient_id INTEGER NOT NULL,
  measurement_type_id INTEGER NOT NULL,
  min REAL,
  max REAL,
  FOREIGN KEY(patient_id) REFERENCES patients(id),
  FOREIGN KEY(measurement_type_id) REFERENCES measurements_types(id),
  PRIMARY KEY (patient_id, measurement_type_id)
);
