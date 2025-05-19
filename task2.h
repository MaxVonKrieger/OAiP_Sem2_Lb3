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


void InsertStr(char** mas);
void printDate(const Date date);
int compareDates(Date date1, Date date2);
Date createDate();
void freeWorker(Worker* worker);
void printContract(Contract contract);
Worker createWorker();
void printWorker(const Worker worker);
int isWorkersFull();
void increaseWorkers();
void addWorkerMenu();
int removeWorker(int index);
void printAllWorkers();
void removeWorkerMenu();
int comparePositions(const void* a, const void* b);
void printAllPositions();
void positionsMenu();
void saveWorkersToFile();
char* trimWhitespace(char* str);
void loadWorkersFromFile();

void showMenu();

