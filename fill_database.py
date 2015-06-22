import os
import sqlite3
import random
import faker  # pypi: fake-factory

MEASUREMENT_TYPES = [('Temperature', u'\N{DEGREE CELSIUS}', 'TEMP', 33,  40),
                     ('Heart Rate', 'BPM', 'PULSE', 60, 100),
                     ('Blood sugar', 'mg/dL', 'SUGAR', 70, 200)]


class DbPopulator(object):
    def __init__(self, db_name="database.db", drop_existing_db=True):
        if drop_existing_db and os.path.exists(db_name):
            os.unlink(db_name)
        self.conn = sqlite3.connect(db_name)
        self.cur = self.conn.cursor()

    def __del__(self):
        self.conn.close()

    def _create_schema(self):
        with open(os.path.join('schemas', 'database.sql')) as schema_file:
            schema_query = schema_file.read()
        self.cur.executescript(schema_query)

    def _create_measurement_types(self):
        self.cur.executemany('INSERT INTO measurements_types (name, unit, token) VALUES (?,?,?)', (m[:3] for m in MEASUREMENT_TYPES))

    def _create_patients(self):
        f = faker.Factory.create('pl_PL')
        patients = [(f.name(), random.randint(100, 200), "040506000405060" + str(i)) for i in xrange(10)]
        self.cur.executemany('INSERT INTO patients (name, room, device_id) VALUES (?,?,?)', patients)
        for pid, _ in enumerate(patients, 1):
            for mid, (_, _, _, vmin, vmax) in enumerate(MEASUREMENT_TYPES, 1):
                # self.cur.execute('INSERT INTO measurements (patient_id, measurement_type, value) VALUES (?,?,?)', (pid, mid, random.randint(80, 120)))
                self.cur.execute('INSERT INTO patients_limits (patient_id, measurement_type_id, min, max) VALUES (?,?,?,?)', (pid, mid, vmin, vmax))

    def commit(self):
        self.conn.commit()

    def run(self):
        self._create_schema()
        self._create_measurement_types()
        self._create_patients()
        self.commit()

def main():
    DbPopulator().run()

if __name__ == "__main__":
    main()
