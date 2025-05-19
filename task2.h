#pragma once
typedef enum {
    FULL_TIME,      // Полная занятость
    PART_TIME,      // Частичная занятость
    CONTRACTOR,     // Внешний сотрудник
    INTERN          // Стажер
} EmploymentType;

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
    EmploymentType employment_type;
    Contract* contracts;
    int contracts_count;
    int contracts_capacity;
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
void initContracts(Worker* worker);
void addContractToWorker(Worker* worker, Contract contract);
void freeContracts(Worker* worker);
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
PositionStats* createPositionsStats(WorkersData* data, int* stats_count);
void printPositionsStats(PositionStats* stats, int stats_count);
void positionsMenu(WorkersData* data);
void showMenu(void);
void saveWorkersToFile(WorkersData* data, const char* filename);
char* trimWhitespace(char* str);
void loadWorkersFromFile(WorkersData* data, const char* filename);
int isWorkerDismissed(Worker* worker, Date current_date);
void removeDismissedWorkers(WorkersData* data, Date current_date);
void removeDismissedMenu(WorkersData* data);
void freeWorkersData(WorkersData* data);
void workerStruct(const char argv);
int intchar(void);
float floatchar(void);
const char* employmentTypeToString(EmploymentType type);