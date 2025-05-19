#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "task2.h"

void InsertStr(char** mas) {
    char a;
    int i = 0;
    *mas = (char*)malloc(1);
    if (*mas == NULL) {
        printf("Ошибка выделения памяти\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        a = _getch();
        if (a == '\r') break;
        if (a == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
                char* temp = (char*)realloc(*mas, i + 1);
                if (temp == NULL) {
                    printf("Ошибка перераспределения памяти\n");
                    exit(EXIT_FAILURE);
                }
                *mas = temp;
            }
            continue;
        }
        printf("%c", a);
        char* temp = (char*)realloc(*mas, i + 2);
        if (temp == NULL) {
            printf("Ошибка перераспределения памяти\n");
            exit(EXIT_FAILURE);
        }
        *mas = temp;
        (*mas)[i] = a;
        i++;
    }
    (*mas)[i] = '\0';
}

void printDate(const Date date) {
    printf("[Дата: %02d.%02d.%04d]", date.day, date.month, date.year);
}

int compareDates(Date date1, Date date2) {
    if (date1.year > date2.year) return 1;
    if (date1.year < date2.year) return -1;
    if (date1.month > date2.month) return 1;
    if (date1.month < date2.month) return -1;
    if (date1.day > date2.day) return 1;
    if (date1.day < date2.day) return -1;
    return 0;
}

Date createDate() {
    Date result;
    do {
        printf("\nВведите день (от 01 до 31): ");
        result.day = intchar();
    } while (result.day < 1 || result.day > 31);

    do {
        printf("\nВведите месяц (от 01 до 12): ");
        result.month = intchar();
    } while (result.month < 1 || result.month > 12);

    do {
        printf("\nВведите год(____): ");
        result.year = intchar();
    } while (result.year < 1900 || result.year > 2100);

    printf("\n");
    return result;
}

Contract createContract() {
    Contract result;

    printf("\nВведите должность: ");
    InsertStr(&result.position);

    do {
        printf("\nВведите ставку (0.1-10.0): ");
        printf("\n");
        result.rate = floatchar();
    } while (result.rate < 0.1f || result.rate > 10.0f);

    do {
        printf("\nВведите ежемесячную зарплату (> 0): ");
        result.monthly_salary = intchar();
    } while (result.monthly_salary <= 0);

    printf("\nДата начала контракта:");
    result.start_date = createDate();

    do {
        printf("\nДата окончания контракта:");
        result.end_date = createDate();
        if (compareDates(result.end_date, result.start_date) < 0) {
            printf("Дата окончания должна быть позже даты начала!\n");
        }
    } while (compareDates(result.end_date, result.start_date) < 0);

    return result;
}

void freeWorker(Worker* worker) {
    if (worker == NULL) return;

    if (worker->full_name != NULL) {
        free(worker->full_name);
        worker->full_name = NULL;
    }

    if (worker->contract.position != NULL) {
        free(worker->contract.position);
        worker->contract.position = NULL;
    }
}

void printContract(Contract contract) {
    printf("Должность: %s\n", contract.position);
    printf("Ставка: %.2f\n", contract.rate);
    printf("Зарплата: %d руб./мес.\n", contract.monthly_salary);
    printf("Дата начала: ");
    printDate(contract.start_date);
    printf("\n");
    printf("Дата окончания: ");
    printDate(contract.end_date);
    printf("\n");
}

Worker createWorker(Worker* existing_workers, int workers_count) {
    Worker result;
    result.full_name = NULL;
    result.contract.position = NULL;

    printf("\nВведите ФИО работника: ");
    InsertStr(&result.full_name);

    printf("\nДата рождения работника:\n");
    result.birth_date = createDate();

    // Ввод табельного номера с проверкой на уникальность
    int is_unique;
    do {
        is_unique = 1; // Предполагаем, что номер уникален

        printf("\nВведите табельный номер (уникальное число > 0): ");
        result.personnel_number = intchar();

        if (result.personnel_number <= 0) {
            printf("Табельный номер должен быть положительным числом!\n");
            continue;
        }

        // Проверяем уникальность номера среди существующих работников
        for (int i = 0; i < workers_count; i++) {
            if (existing_workers[i].personnel_number == result.personnel_number) {
                printf("Ошибка: работник с табельным номером %d уже существует!\n",
                    result.personnel_number);
                is_unique = 0;
                break;
            }
        }

        if (!is_unique) {
            printf("Пожалуйста, введите другой табельный номер.\n");
        }
    } while (result.personnel_number <= 0 || !is_unique);

    printf("\nВвод данных контракта:\n");
    result.contract = createContract();

    return result;
}

void printWorker(const Worker worker) {
    printf("\n=== Информация о работнике ===\n");
    printf("ФИО: %s\n", worker.full_name);
    printf("Дата рождения: ");
    printDate(worker.birth_date);
    printf("\n");
    printf("Табельный номер: %d\n", worker.personnel_number);
    printf("\n--- Данные контракта ---\n");
    printContract(worker.contract);
    printf("===========================\n");
}

void initWorkersData(WorkersData* data) {
    data->workers_capacity = 10;
    data->workers_count = 0;
    data->workers = (Worker*)malloc(data->workers_capacity * sizeof(Worker));
    if (data->workers == NULL) {
        printf("Ошибка выделения памяти\n");
        exit(EXIT_FAILURE);
    }
}

void increaseWorkersCapacity(WorkersData* data) {
    data->workers_capacity++;
    Worker* temp = (Worker*)realloc(data->workers, data->workers_capacity * sizeof(Worker));
    if (temp == NULL) {
        printf("Ошибка перераспределения памяти\n");
        exit(EXIT_FAILURE);
    }
    data->workers = temp;
}

void addWorkerMenu(WorkersData* data) {
    // Проверяем и увеличиваем емкость при необходимости
    if (data->workers_count >= data->workers_capacity) {
        increaseWorkersCapacity(data);
        if (data->workers == NULL) {
            printf("Ошибка: не удалось увеличить емкость массива работников!\n");
            return;
        }
    }

    // Создаем нового работника с проверкой уникальности номера
    Worker new_worker = createWorker(data->workers, data->workers_count);

    // Проверяем, что создание прошло успешно (указатели не NULL)
    if (new_worker.full_name == NULL || new_worker.contract.position == NULL) {
        printf("Ошибка: не удалось создать работника!\n");
        freeWorker(&new_worker); // Освобождаем частично созданного работника
        return;
    }

    // Добавляем работника в массив
    data->workers[data->workers_count] = new_worker;
    data->workers_count++;

    printf("\nРаботник %s успешно добавлен! Табельный номер: %d\n",
        new_worker.full_name, new_worker.personnel_number);
}


int removeWorker(WorkersData* data, int index) {
    if (index < 0 || index >= data->workers_count || data->workers == NULL) {
        printf("Ошибка: неверный индекс работника\n");
        return 0;
    }

    freeWorker(&data->workers[index]);

    for (int i = index; i < data->workers_count - 1; i++) {
        freeWorker(&data->workers[i]);
        data->workers[i] = data->workers[i + 1];
        data->workers[i + 1].full_name = NULL;
        data->workers[i + 1].contract.position = NULL;
    }

    data->workers_count--;
    return 1;
}

void printAllWorkers(WorkersData* data) {
    if (data->workers_count == 0) {
        printf("\n=== Нет данных о работниках ===\n");
        return;
    }

    printf("\n=== СПИСОК РАБОТНИКОВ (%d) ===\n", data->workers_count);
    for (int i = 0; i < data->workers_count; i++) {
        printf("\n[Работник #%d]\n", i + 1);
        printWorker(data->workers[i]);
    }
    printf("\n=== Конец списка ===\n");
}

void removeWorkerMenu(WorkersData* data) {
    if (data->workers_count == 0) {
        printf("\nСписок работников пуст!\n");
        return;
    }

    printAllWorkers(data);

    int index;
    do {
        printf("\nВведите индекс работника для удаления (0-%d) или -1 для отмены: ", data->workers_count);
        index = intchar();

        if (index == -1) {
            printf("Удаление отменено.\n");
            return;
        }

        if (index < 0 || index > data->workers_count) {
            printf("Ошибка: неверный индекс! Попробуйте снова.\n");
        }
    } while (index < 0 || index > data->workers_count);

    printf("\nВы действительно хотите удалить работника %s? (1 - да, 0 - нет): ",
        data->workers[index - 1].full_name);

    int confirm;
    do {
        confirm = intchar();
        if (confirm != 0 && confirm != 1) {
            printf("Введите 1 для подтверждения или 0 для отмены: ");
        }
    } while (confirm != 0 && confirm != 1);

    if (confirm == 1) {
        if (removeWorker(data, index - 1)) {
            printf("\nРаботник успешно удален!\n");
            printf("Осталось работников: %d\n", data->workers_count);
        }
        else {
            printf("\nОшибка при удалении работника!\n");
        }
    }
    else {
        printf("Удаление отменено.\n");
    }
}

int comparePositions(const void* a, const void* b) {
    const PositionStats* pa = (const PositionStats*)a;
    const PositionStats* pb = (const PositionStats*)b;

    if (pa->avg_salary > pb->avg_salary) return -1;
    if (pa->avg_salary < pb->avg_salary) return 1;
    return 0;
}

void printAllPositions(WorkersData* data) {
    if (data->workers_count == 0) {
        printf("Нет данных о работниках!\n");
        return;
    }

    PositionStats* stats = NULL;
    int stats_count = 0;
    int stats_capacity = 10;
    stats = (PositionStats*)malloc(stats_capacity * sizeof(PositionStats));
    if (stats == NULL) {
        printf("Ошибка выделения памяти!\n");
        return;
    }

    for (int i = 0; i < data->workers_count; i++) {
        char* current_pos = data->workers[i].contract.position;
        int found = 0;

        for (int j = 0; j < stats_count; j++) {
            if (strcmp(stats[j].position_name, current_pos) == 0) {
                stats[j].avg_salary = (stats[j].avg_salary * stats[j].count +
                    data->workers[i].contract.monthly_salary) /
                    (stats[j].count + 1);
                stats[j].count++;
                found = 1;
                break;
            }
        }

        if (!found) {
            if (stats_count >= stats_capacity) {
                stats_capacity *= 2;
                PositionStats* temp = (PositionStats*)realloc(stats,
                    stats_capacity * sizeof(PositionStats));
                if (temp == NULL) {
                    printf("Ошибка перераспределения памяти!\n");
                    free(stats);
                    return;
                }
                stats = temp;
            }

            stats[stats_count].position_name = _strdup(current_pos);
            stats[stats_count].avg_salary = data->workers[i].contract.monthly_salary;
            stats[stats_count].count = 1;
            stats_count++;
        }
    }

    qsort(stats, stats_count, sizeof(PositionStats), comparePositions);

    printf("\n=== Все должности (%d) ===\n", stats_count);
    printf("%-30s %-15s %-10s\n", "Должность", "Средняя зарплата", "Кол-во");
    printf("------------------------------------------------\n");

    for (int i = 0; i < stats_count; i++) {
        printf("%-30s %-15.2f %-10d\n",
            stats[i].position_name,
            stats[i].avg_salary,
            stats[i].count);
    }
    printf("================================================\n");

    for (int i = 0; i < stats_count; i++) {
        free(stats[i].position_name);
    }
    free(stats);
}

void positionsMenu(WorkersData* data) {
    if (data->workers_count == 0) {
        printf("Нет данных о работниках!\n");
        return;
    }

    printAllPositions(data);
}

void showMenu() {
    printf("\n1. Добавить\n2. Удалить\n3. Список\n4. Топ\n5. Удалить уволенных\n6. Сохранить в файл\n7. Загрузить из файла\n0. Выход\nВыбор: ");
}

void saveWorkersToFile(WorkersData* data, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла для записи!\n");
        return;
    }

    for (int i = 0; i < data->workers_count; i++) {
        Worker* worker = &data->workers[i];
        Contract* contract = &worker->contract;

        fprintf(file, "%s | %d | %d | %d | %d | %s | %.2f | %d | %d | %d | %d | %d | %d | %d\n",
            worker->full_name,
            worker->birth_date.day, worker->birth_date.month, worker->birth_date.year,
            worker->personnel_number,
            contract->position,
            contract->rate,
            contract->monthly_salary,
            contract->start_date.day, contract->start_date.month, contract->start_date.year,
            contract->end_date.day, contract->end_date.month, contract->end_date.year);
    }

    fclose(file);
    printf("Данные успешно сохранены в файл %s\n", filename);
}

char* trimWhitespace(char* str) {
    if (str == NULL) return NULL;

    char* end;

    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';

    return str;
}

void loadWorkersFromFile(WorkersData* data, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Файл %s не найден или не может быть открыт!\n", filename);
        return;
    }

    for (int i = 0; i < data->workers_count; i++) {
        freeWorker(&data->workers[i]);
    }
    free(data->workers);

    data->workers_capacity = 10;
    data->workers_count = 0;
    data->workers = (Worker*)malloc(data->workers_capacity * sizeof(Worker));
    if (data->workers == NULL) {
        printf("Ошибка выделения памяти!\n");
        fclose(file);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (data->workers_count >= data->workers_capacity) {
            data->workers_capacity++;
            Worker* temp = (Worker*)realloc(data->workers, data->workers_capacity * sizeof(Worker));
            if (temp == NULL) {
                printf("Ошибка перераспределения памяти!\n");
                break;
            }
            data->workers = temp;
        }

        Worker* current = &data->workers[data->workers_count];
        current->full_name = NULL;
        current->contract.position = NULL;

        char* token = strtok(line, "|");
        if (token == NULL) continue;

        token = trimWhitespace(token);
        current->full_name = (char*)malloc(strlen(token) + 1);
        strcpy(current->full_name, token);

        token = strtok(NULL, "|"); current->birth_date.day = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->birth_date.month = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->birth_date.year = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->personnel_number = atoi(trimWhitespace(token));

        token = strtok(NULL, "|");
        token = trimWhitespace(token);
        current->contract.position = (char*)malloc(strlen(token) + 1);
        strcpy(current->contract.position, token);

        token = strtok(NULL, "|"); current->contract.rate = atof(trimWhitespace(token));
        token = strtok(NULL, "|"); current->contract.monthly_salary = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->contract.start_date.day = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->contract.start_date.month = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->contract.start_date.year = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->contract.end_date.day = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->contract.end_date.month = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->contract.end_date.year = atoi(trimWhitespace(token));

        data->workers_count++;
    }

    fclose(file);
    printf("Загружено %d работников из файла.\n", data->workers_count);
}

void removeDismissedWorkers(WorkersData* data, Date current_date) {
    if (data->workers_count == 0) {
        printf("Нет работников для проверки!\n");
        return;
    }

    int removed_count = 0;

    for (int i = data->workers_count - 1; i >= 0; i--) {
        if (compareDates(data->workers[i].contract.end_date, current_date) < 0) {
            printf("Удаляем уволенного работника: %s (контракт закончился %02d.%02d.%04d)\n",
                data->workers[i].full_name,
                data->workers[i].contract.end_date.day,
                data->workers[i].contract.end_date.month,
                data->workers[i].contract.end_date.year);

            if (removeWorker(data, i)) {
                removed_count++;
            }
        }
    }

    if (removed_count == 0) {
        printf("Нет уволенных работников на указанную дату.\n");
    }
    else {
        printf("Удалено %d уволенных работников. Осталось %d работников.\n",
            removed_count, data->workers_count);
    }
}

void removeDismissedMenu(WorkersData* data) {
    if (data->workers_count == 0) {
        printf("Нет работников для проверки!\n");
        return;
    }

    printf("\n=== Удаление уволенных работников ===\n");
    printf("Введите текущую дату для проверки:\n");
    Date current_date = createDate();

    removeDismissedWorkers(data, current_date);
}

void freeWorkersData(WorkersData* data) {
    for (int i = 0; i < data->workers_count; i++) {
        freeWorker(&data->workers[i]);
    }
    free(data->workers);
    data->workers = NULL;
    data->workers_count = 0;
    data->workers_capacity = 0;
}



void workerStruct(const char argv)
{
    int exit_submenu = 0;
    WorkersData workers_data;
    initWorkersData(&workers_data);
    do
    {
        exit_submenu = 0;
        showMenu();
        int answer = intchar();
        switch (answer)
        {
        case 1:
            addWorkerMenu(&workers_data);
            break;
        case 2:
            removeWorkerMenu(&workers_data);
            break;
        case 3:
            printAllWorkers(&workers_data);
            break;
        case 4:
            positionsMenu(&workers_data);
            break;
        case 5:
            removeDismissedMenu(&workers_data);
            break;
        case 6:
            saveWorkersToFile(&workers_data, "workers.txt");
            break;
        case 7:
            loadWorkersFromFile(&workers_data, "workers.txt");
            break;
        case 0:
            printf("Выход из программы...\n");
            exit_submenu = 1;
            break;
        default:
            printf("Неверный выбор!\n");
            break;
        }
    } while (!exit_submenu);
    freeWorkersData(&workers_data);
}
