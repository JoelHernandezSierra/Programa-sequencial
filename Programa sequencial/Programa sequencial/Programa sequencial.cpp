#include <iostream>
#include <sqlite3.h>

using namespace std;

// Función para abrir/crear la base de datos
sqlite3* openDatabase(const char* filename) {
    sqlite3* db;
    int exit = sqlite3_open(filename, &db);

    if (exit) {
        cerr << "Error abriendo/creando la base de datos: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    else {
        cout << "Conexión exitosa a la base de datos." << endl;
        return db;
    }
}

// Función para ejecutar sentencias SQL sin resultados (como CREATE, INSERT, UPDATE)
void executeSQL(sqlite3* db, const char* sql) {
    char* errMsg;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "Error al ejecutar SQL: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    else {
        cout << "SQL ejecutado correctamente." << endl;
    }
}

// Función para mostrar los resultados de una consulta
int selectCallback(void* NotUsed, int argc, char** argv, char** colName) {
    for (int i = 0; i < argc; i++) {
        cout << colName[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << endl;
    return 0;
}

// Función para realizar una consulta SELECT
void selectData(sqlite3* db, const char* sql) {
    char* errMsg;
    int rc = sqlite3_exec(db, sql, selectCallback, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "Error en la consulta SELECT: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    else {
        cout << "Consulta SELECT ejecutada correctamente." << endl;
    }
}

// Función para cerrar la conexión a la base de datos
void closeDatabase(sqlite3* db) {
    sqlite3_close(db);
    cout << "Conexión a la base de datos cerrada." << endl;
}

int main() {
    // Abrimos la base de datos
    sqlite3* db = openDatabase("otra_base_de_datos.db");
    if (!db) return -1;  // Si no se puede abrir, salir

    // Creamos una tabla llamada 'empleados'
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS empleados (id INTEGER PRIMARY KEY, nombre TEXT, salario REAL);";
    executeSQL(db, createTableSQL);

    // Insertamos datos en la tabla
    const char* insertSQL = "INSERT INTO empleados (nombre, salario) VALUES ('Ana', 2500.50);";
    executeSQL(db, insertSQL);

    // Insertamos otra
    const char* insertSQL2 = "INSERT INTO empleados (nombre, salario) VALUES ('Carlos', 3200.75);";
    executeSQL(db, insertSQL2);

    // Consultamos los datos
    cout << "Datos actuales de la tabla 'empleados':" << endl;
    const char* selectSQL = "SELECT * FROM empleados;";
    selectData(db, selectSQL);

    // Actualizamos los datos
    const char* updateSQL = "UPDATE empleados SET salario = 3500 WHERE nombre = 'Ana';";
    executeSQL(db, updateSQL);

    // Mostramos los datos actualizados
    cout << "Datos después de la actualización:" << endl;
    selectData(db, selectSQL);

    // Cerramos la base de datos
    closeDatabase(db);

    return 0;
}