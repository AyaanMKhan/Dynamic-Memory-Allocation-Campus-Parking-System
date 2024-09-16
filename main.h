#define MAXLEN 21

typedef struct RegisteredVehicle{ //for one registered vehicle

    char* license_plate; //to be used for string license plate# of the vehicle
    char* owner_name; //to be used for storing owner name

} RegisteredVehicle;


typedef struct Garage{ //for one garage

    char* garage_name; //to be used for garage name
    int total_capacity; //total capacity of the garage
    int current_count; // number of vehicles in the garage in a particular time
    RegisteredVehicle** parked_vehicles; //list of parked vehicles in a particular time

} Garage;


typedef struct Campus{

    Garage** garages; //list of garages in campus
    int total_garages; //number of garages in the campus
    RegisteredVehicle** registered_vehicles; //list of registered vehicles
    int total_registered_vehicles; //total number of registered vehicles

} Campus;

Garage* createGarage(const char* name, int capacity);
RegisteredVehicle* createRegisteredVechile(const char* license, const char* owner);
void registerVechile(Campus *campus, const char* license, const char* owner);
void parkVehicle(Garage *garage, RegisteredVehicle* vehicle);
int removeVehicleFromGarage(Garage *garage, const char *license);
RegisteredVehicle* searchVehicleByLicense(const Campus* campus, const char* license);
int countTotalVehicles(const Campus* campus);
int resizeGarage(Garage *garage, int new_capacity);
int relocateVehicle(Campus *campus, const char* license, const char* target_garage_num);
int removeGarage(Campus *campus, const char* garage_name);
void displayVehicleByOwner(const Campus *campus, const char *owner_name);
void generateGarageUtilizationReport(const Campus* campus);
void freeMemory(Campus *campus);