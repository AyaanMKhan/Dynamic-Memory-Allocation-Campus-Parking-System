/*
This program is written by: Ayaan Khan */

// All of the library files that we will need for the assingment
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
RegisteredVehicle* createRegisteredVehicle(const char* license, const char* owner);
void registerVehicle(Campus *campus, const char* license, const char* owner);
void parkVehicle(Garage *garage, RegisteredVehicle* vehicle);
int removeVehicleFromGarage(Garage *garage, const char *license);
RegisteredVehicle* searchVehicleByLicense(const Campus* campus, const char* license);
int countTotalVehicles(const Campus* campus);
int resizeGarage(Garage *garage, int new_capacity);
Garage* garageFinder(Campus *campus, const  char* garage_name);
RegisteredVehicle* registerdVehicleFinder(Campus *campus, const char* license);
int relocateVehicle(Campus *campus, const char* license, const char* target_garage_num);
int removeGarage(Campus *campus, const char* garage_name);
void displayVehicleByOwner(const Campus *campus, const char *owner_name);
void generateGarageUtilizationReport(const Campus* campus);
void freeMemory(Campus *campus);


Garage* createGarage(const char* name, int capacity){

    // Makes the garage and vehicle array

    Garage* newGarage = (Garage*)malloc(sizeof(Garage));

    int len = strlen(name) + 1;
    newGarage->garage_name = (char*)malloc(len * sizeof(char));
    strcpy(newGarage->garage_name, name);

    newGarage->total_capacity = capacity;
    newGarage->current_count = 0;

    newGarage->parked_vehicles = (RegisteredVehicle**)malloc(capacity * sizeof(RegisteredVehicle*));

    return newGarage;

}

RegisteredVehicle* createRegisteredVehicle(const char* license, const char* owner){

    //Makes the vehicle license and owner arrays
    RegisteredVehicle* RV = (RegisteredVehicle*)malloc(sizeof(RegisteredVehicle));

    int license_len = strlen(license) + 1;
    RV->license_plate = (char*)malloc(license_len * sizeof(char));
    strcpy(RV->license_plate, license);

    int owner_len = strlen(owner) + 1;
    RV->owner_name = (char*)malloc(owner_len * sizeof(char));
    strcpy(RV->owner_name, owner);

    return RV;
}



void registerVehicle(Campus *campus, const char* license, const char* owner){

    // Create the vehicle, then register it increasing the total number of vehicles
    RegisteredVehicle *newRV = createRegisteredVehicle(license, owner);
    int capacity = campus->total_registered_vehicles + 1;
    campus->registered_vehicles = (RegisteredVehicle**)realloc(campus->registered_vehicles, capacity * sizeof(RegisteredVehicle*));

    campus->registered_vehicles[campus->total_registered_vehicles] = newRV;
    campus->total_registered_vehicles += 1;

    printf("REGISTERED\n");

}


void parkVehicle(Garage *garage, RegisteredVehicle* vehicle){

    // Putting a vehicle into the last place of the garage based on current capacity
    if(garage->current_count >= garage->total_capacity){
        printf("FULL\n");
    } else {
        garage->parked_vehicles[garage->current_count] = vehicle;
        garage->current_count += 1;
        printf("PARKED\n");
    }
}



int removeVehicleFromGarage(Garage *garage, const char *license){
    
    int flag = 0;
    // Finds the license plate of the car we looking for, then makes that vehicle NULL, removing it, then shifting the vehicles to fill the slots
    for(int i = 0; i < garage->total_capacity; i++){
        if(strcmp(garage->parked_vehicles[i]->license_plate, license) == 0){
            flag = 1;

            for(int j = i; j < garage->current_count; j++){
                garage->parked_vehicles[j] = garage->parked_vehicles[j+1];
            }

            garage->current_count -= 1;
            printf("REMOVED FROM %s\n", garage->garage_name);
            

            break;
        }

    }

    if(!flag){
        printf("NOT FOUND IN CAMPUS\n");
        return 0;
    }

    return 1;
}



RegisteredVehicle* searchVehicleByLicense(const Campus* campus, const char* license){


    // Find vehicle by their license plate
    for(int i = 0; i < campus->total_garages; i++){
        Garage *garage = campus->garages[i];
        for(int j = 0; j < garage->current_count; j++){
            if(strcmp(garage->parked_vehicles[j]->license_plate, license) == 0){
                return garage->parked_vehicles[j];
            }
        }
    }

    return NULL;
}


int countTotalVehicles(const Campus* campus){

    int total = 0;
    // Keeps track of the amount of vehicles
    for(int i = 0; i < campus->total_garages; i++){

        total += campus->garages[i]->current_count;
    }

    return total;
}

int resizeGarage(Garage *garage, int new_capacity){

    if(new_capacity < garage->current_count){
        printf("FAIL. TOO SMALL TO ACCOMMODATE EXISTING VEHICLES.\n");
        return 0;
    }
    // Makes more space in the garage, based on the new capacity needed
    RegisteredVehicle** new_garage = (RegisteredVehicle**)realloc(garage->parked_vehicles, new_capacity * sizeof(RegisteredVehicle*));
    garage->parked_vehicles = new_garage;
    garage->total_capacity = new_capacity;

    printf("SUCCESS\n");


    return 1;

}

// This is a helper function made to find garage based on its name, just to not redo for loops
Garage* garageFinder(Campus *campus, const  char* garage_name){
    for(int i = 0; i < campus->total_garages; i++){
        if(strcmp(campus->garages[i]->garage_name, garage_name) == 0){
            return campus->garages[i];
        }
    }

    return NULL;
}

// Helper function to find registered vehicle
RegisteredVehicle* registerdVehicleFinder(Campus *campus, const char* license){
    for(int i = 0; i < campus->total_registered_vehicles; i++){
        if(strcmp(campus->registered_vehicles[i]->license_plate, license) == 0){
            return campus->registered_vehicles[i];
        }
    }
    return NULL;
}

int relocateVehicle(Campus *campus, const char* license, const char* target_garage_num){

    Garage *current = NULL;
    RegisteredVehicle *vehicle = NULL;
    for(int i = 0; i < campus->total_garages; i++){
        Garage *garage = campus->garages[i];

        // getting the garage we want to put the vehicle in
        for(int j = 0; j < garage->current_count; j++){
            if(strcmp(garage->parked_vehicles[j]->license_plate, license) == 0){
                current = garage;
                vehicle = garage->parked_vehicles[j];
                break;
            }
        }
        if(vehicle != NULL){
            break;
        }
    }

    // Checkign for the garage and vehicle

    Garage *temp = garageFinder(campus, target_garage_num);
    if(temp == NULL){
        printf("%s NOT FOUND\n", target_garage_num);
        return 0;
    }

    if(vehicle == NULL){
        printf("%s NOT IN CAMPUS.\n", license);
        return 0;
    }

    

    // if garage is full cannot relocate

    if(temp->current_count >= temp->total_capacity){
        printf("%s IS FULL.\n", target_garage_num);
        return 0;
    }

    // Relocating the parked vehicles


    for(int i = 0; i < current->current_count; i++){
        if(strcmp(current->parked_vehicles[i]->license_plate, license) == 0){
            
            for(int j = i; j < current->current_count-1; j++){
                current->parked_vehicles[j] = current->parked_vehicles[j+1];
            }

            current->current_count -= 1;
            break;
        }
    }

    // adds to the end, update count
    temp->parked_vehicles[temp->current_count] = vehicle;
    temp->current_count += 1;

    printf("PARKED\n");
    printf("RELOCATION SUCCESSFUL.\n");
    return 1;
    
    
}

int removeGarage(Campus *campus, const char* garage_name){

    // Frees the memory of the garage we want to remove, then decreases the total garages we have
    int flag = 0;
    for(int i = 0; i < campus->total_garages; i++){
        if(strcmp(campus->garages[i]->garage_name, garage_name) == 0){
            flag = 1;

            free(campus->garages[i]->garage_name);
            free(campus->garages[i]->parked_vehicles);
            free(campus->garages[i]);

            for(int j = i; j < campus->total_garages; j++){
                campus->garages[j] = campus->garages[j+1];
            }

            campus->total_garages -= 1;

            printf("%s REMOVED\n", garage_name);
            break;
        }
    }

    if(!flag){
        printf("%s NOT FOUND\n", garage_name);
        return 0;
    }
    return 1;

}

void displayVehicleByOwner(const Campus *campus, const char *owner_name){

    int flag = 0;

    for(int i = 0; i < campus->total_registered_vehicles; i++){
        RegisteredVehicle *vehicle = campus->registered_vehicles[i];
        // Using 2 flag variables in order to know whether my code goes in the nested for loops, and then gets the value of the garage_name and license to print
        if(strcmp(vehicle->owner_name, owner_name) == 0){
            flag = 1;
            int flag_2 = 0;
            char* garageName = NULL;
            for(int j = 0; j < campus->total_garages; j++){
                Garage *garage = campus->garages[j];

                // gets the garage name and then breaks out, and prints
                for(int k = 0; k < garage->current_count; k++){
                    if(strcmp(garage->parked_vehicles[k]->license_plate, vehicle->license_plate) == 0){
                        flag_2 = 1;
                        garageName = garage->garage_name;
                        break;
                    }
                }

                if(flag_2){
                    break;
                }
            }
            if(flag_2){
                printf("%s %s\n", vehicle->license_plate, garageName);
            } else {
                printf("%s NOT ON CAMPUS\n", vehicle->license_plate);
            }
        }
    }
    if(!flag){
        printf("NO REGISTERED CAR BY THIS OWNER\n");
    }


}

void generateGarageUtilizationReport(const Campus* campus){

    int least_utilized = 0;
    float least_utilized_percent = 100.0;
    
    // Prints the report of all the garages and says the least used
    for(int i = 0; i < campus->total_garages; i++){
        float utilization = (float)campus->garages[i]->current_count / campus->garages[i]->total_capacity;
        printf("Garage: %s, Capacity: %d, Occupied: %d, Utilization: %0.2f%%\n", campus->garages[i]->garage_name, campus->garages[i]->total_capacity, campus->garages[i]->current_count, utilization*100);

        // Gets the least
        if(utilization < least_utilized_percent){
            least_utilized_percent = utilization;
            least_utilized = i;
        }
    }

    printf("Least Utilized: %s\n", campus->garages[least_utilized]->garage_name);
}

void freeMemory(Campus *campus){

    // Free all the memory

    for(int i = 0; i < campus->total_garages; i++){
        Garage *garage = campus->garages[i];
        free(garage->parked_vehicles);
        free(garage->garage_name);
        free(garage);
    }

    for(int i = 0; i < campus->total_registered_vehicles; i++){
        RegisteredVehicle *vehilce = campus->registered_vehicles[i];
        free(vehilce->license_plate);
        free(vehilce->owner_name);
        free(vehilce);
    }

    free(campus->garages);
    free(campus->registered_vehicles);

}



int main(){

    //atexit(report_mem_leak);

    int garage_num, vehicle_num, command_num;
    scanf("%d %d %d", &garage_num, &vehicle_num, &command_num);

    // Making all the boxes for campus, garage, and registered vehicles
    int capacity;
    Campus *campus = (Campus*)malloc(sizeof(Campus));
    campus->total_garages = garage_num;
    campus->total_registered_vehicles = vehicle_num;

    campus->garages = (Garage**)malloc(garage_num * sizeof(Garage*));
    campus->registered_vehicles = (RegisteredVehicle**)malloc(vehicle_num * sizeof(RegisteredVehicle*));

    for(int i = 0; i < garage_num; i++){
        char garage_name[MAXLEN+1];
        int capacity;
        scanf("%s %d", garage_name, &capacity);
        campus->garages[i] = createGarage(garage_name, capacity);
    }

    for(int i = 0; i < vehicle_num; i++){
        char license[MAXLEN+1], owner[MAXLEN+1];
        scanf("%s %s", license, owner);
        campus->registered_vehicles[i] = createRegisteredVehicle(license, owner);
    }

    for(int i = 0; i < command_num; i++){
        char choice[50];
        scanf("%s", choice);
        // This is where all the commands go that the user can use
        if(strcmp("PARK", choice) == 0){
            char license[MAXLEN+1], garage_name[MAXLEN+1];
            scanf("%s %s", license, garage_name);

            // Find the vehicle and garage, and then park it using the function
            RegisteredVehicle *vehicle = registerdVehicleFinder(campus, license);
            Garage *garage = garageFinder(campus, garage_name);

            if(vehicle != NULL && garage != NULL){

                RegisteredVehicle *temp = searchVehicleByLicense(campus, license);
                if(temp != NULL){
                    printf("PARKED\n");
                } else {
                    parkVehicle(garage, vehicle);
                }

            }
            // Prints everything
        } else if(strcmp("UTILIZATION_REPORT", choice) == 0){
            generateGarageUtilizationReport(campus);
        } else if(strcmp("RESIZE", choice) == 0){
            // Changes the size of the garage
            char garage_name[MAXLEN];
            int capacity_new;
            scanf("%s %d", garage_name, &capacity_new);

            Garage *garage = garageFinder(campus, garage_name);

            if(garage != NULL){
                resizeGarage(garage, capacity_new);
            }

        } else if(strcmp("SEARCH_OWNER", choice) == 0){
            // Get owner and display their license plate and garage name
            char owner[MAXLEN+1];
            scanf("%s", owner);
            displayVehicleByOwner(campus, owner);

        } else if(strcmp("RELOCATE", choice) == 0){
            // Change garage
            char license[MAXLEN+1], garage_name[MAXLEN+1];
            scanf("%s %s", license, garage_name);
            relocateVehicle(campus, license, garage_name);

        } else if(strcmp("COUNT_TOTAL", choice) == 0){
            // Gets the total of vehicles
            int total = countTotalVehicles(campus);
            printf("%d\n", total);

        } else if(strcmp("REGISTER_VEHICLE", choice) == 0){
            // Makes new vehicles and adds them to the array, increasing the count
            char license[MAXLEN+1], owner[MAXLEN+1];
            scanf("%s %s", license, owner);
            registerVehicle(campus, license, owner);

        } else if(strcmp("REMOVE_VEHICLE_GARAGE", choice) == 0){
            // Shifts the place of the vehicles, lowers the count
            char license[MAXLEN+1];
            scanf("%s", license);

            Garage *temp = NULL;
            for(int i = 0; i < campus->total_garages; i++){
                Garage *garage = campus->garages[i];
                // getting the license plate to pass in the function
                for(int j = 0; j < garage->current_count; j++){
                    if(strcmp(garage->parked_vehicles[j]->license_plate, license) == 0){
                        temp = garage;
                        break;
                    }
                }
                if(temp != NULL){
                    break;
                }
            }
            // removing the vehicle
            if(temp != NULL){
                removeVehicleFromGarage(temp, license);
            } else {
                printf("NOT FOUND IN CAMPUS\n");
            }

        } else if(strcmp("REMOVE_GARAGE", choice) == 0){
            // free garage memory, shift the array and then decrease the count
            char garage_name[MAXLEN+1];
            scanf("%s", garage_name);
            removeGarage(campus, garage_name);

        }
    }

    freeMemory(campus);
    free(campus);

    return 0;
}