typedef struct {
    char usuario[50];
    char password[50];
} Admin;

typedef struct {
    char nombreLocalComercial[100];
    char telefono[20];
    char cedulaJuridica[20];
    char horarioAtencion[100];
    int numeroSiguientePedido;
    Admin admin;
} Config;


int cargarConfig(const char* archivo, Config* cfg);
int guardarConfig(const char* archivo, Config* cfg);
int actualizarNumeroSiguientePedido(Config* cfg);