#pragma once
typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char* position_name;
    float avg_salary;
    int count;
} PositionStats;

typedef struct {
    char* position;
    float rate;
    int monthly_salary;
    Date start_date;
    Date end_date;
} Contract;

typedef struct {
    char* full_name;
    Date birth_date;
    int personnel_number;
    Contract contract;
} Worker;

typedef struct {
    Worker* workers;
    int workers_count;
    int workers_capacity;
} WorkersData;

void InsertStr(char** mas);
void printDate(const Date date);
int compareDates(Date date1, Date date2);
Date createDate(void);
Contract createContract(void);
void freeWorker(Worker* worker);
void printContract(Contract contract);
Worker createWorker(Worker* existing_workers, int workers_count);
void printWorker(const Worker worker);
void initWorkersData(WorkersData* data);
void increaseWorkersCapacity(WorkersData* data);
void addWorkerMenu(WorkersData* data);
int removeWorker(WorkersData* data, int index);
void printAllWorkers(WorkersData* data);
void removeWorkerMenu(WorkersData* data);
int comparePositions(const void* a, const void* b);
void printAllPositions(WorkersData* data);
void positionsMenu(WorkersData* data);
void showMenu(void);
void saveWorkersToFile(WorkersData* data, const char* filename);
char* trimWhitespace(char* str);
void loadWorkersFromFile(WorkersData* data, const char* filename);
void removeDismissedWorkers(WorkersData* data, Date current_date);
void removeDismissedMenu(WorkersData* data);
void freeWorkersData(WorkersData* data);
void workerStruct(const char argv);
