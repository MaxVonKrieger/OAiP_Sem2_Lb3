#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "task2.h"
#include <ctype.h>


const char* employmentTypeToString(EmploymentType type) {
    switch (type) {
    case FULL_TIME: return "Полная занятость";
    case PART_TIME: return "Частичная занятость";
    case CONTRACTOR: return "Внешний сотрудник";
    case INTERN: return "Стажер";
    default: return "Неизвестный тип";
    }
}

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
    result.position = NULL;

    printf("\nВведите должность: ");
    InsertStr(&result.position);

    do {
        printf("\nВведите ставку (0.1-10.0): ");
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

void initContracts(Worker* worker) {
    worker->contracts_capacity = 2;
    worker->contracts_count = 0;
    worker->contracts = (Contract*)malloc(worker->contracts_capacity * sizeof(Contract));
    if (worker->contracts == NULL) {
        printf("Ошибка выделения памяти для контрактов\n");
        exit(EXIT_FAILURE);
    }
}

void addContractToWorker(Worker* worker, Contract contract) {
    if (worker->contracts_count >= worker->contracts_capacity) {
        worker->contracts_capacity *= 2;
        Contract* temp = (Contract*)realloc(worker->contracts, worker->contracts_capacity * sizeof(Contract));
        if (temp == NULL) {
            printf("Ошибка перераспределения памяти для контрактов\n");
            exit(EXIT_FAILURE);
        }
        worker->contracts = temp;
    }
    worker->contracts[worker->contracts_count] = contract;
    worker->contracts_count++;
}

void freeContracts(Worker* worker) {
    if (worker->contracts != NULL) {
        for (int i = 0; i < worker->contracts_count; i++) {
            if (worker->contracts[i].position != NULL) {
                free(worker->contracts[i].position);
            }
        }
        free(worker->contracts);
        worker->contracts = NULL;
    }
    worker->contracts_count = 0;
    worker->contracts_capacity = 0;
}

Worker createWorker(Worker* existing_workers, int workers_count) {
    Worker result;
    result.full_name = NULL;
    initContracts(&result);

    printf("\nВведите ФИО работника: ");
    InsertStr(&result.full_name);

    printf("\nДата рождения работника:\n");
    result.birth_date = createDate();

    // Ввод типа занятости
    printf("\nВыберите тип занятости:\n");
    printf("1. Полная занятость\n");
    printf("2. Частичная занятость\n");
    printf("3. Внешний сотрудник\n");
    printf("4. Стажер\n");
    printf("Выбор: ");

    int type_choice;
    do {
        type_choice = intchar();
        if (type_choice < 1 || type_choice > 4) {
            printf("Неверный выбор! Введите число от 1 до 4: ");
        }
    } while (type_choice < 1 || type_choice > 4);

    result.employment_type = (EmploymentType)(type_choice - 1);

    // Ввод табельного номера с проверкой на уникальность
    int is_unique;
    do {
        is_unique = 1;
        printf("\nВведите табельный номер (уникальное число > 0): ");
        result.personnel_number = intchar();

        if (result.personnel_number <= 0) {
            printf("Табельный номер должен быть положительным числом!\n");
            continue;
        }

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

    // Ввод контрактов
    char add_more;
    do {
        printf("\nВвод данных контракта #%d:\n", result.contracts_count + 1);
        Contract new_contract = createContract();
        addContractToWorker(&result, new_contract);

        if (result.contracts_count < 10) {
            printf("\nДобавить еще один контракт для этого работника? (y/n): ");
            add_more = _getch();
            printf("%c\n", add_more);
        }
        else {
            printf("\nДостигнуто максимальное количество контрактов (10)\n");
            add_more = 'n';
        }
    } while ((add_more == 'y' || add_more == 'Y') && result.contracts_count < 10);

    return result;
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

void printWorker(const Worker worker) {
    printf("\n=== Информация о работнике ===\n");
    printf("ФИО: %s\n", worker.full_name);
    printf("Дата рождения: ");
    printDate(worker.birth_date);
    printf("\n");
    printf("Табельный номер: %d\n", worker.personnel_number);
    printf("Тип занятости: %s\n", employmentTypeToString(worker.employment_type));

    printf("\n--- Данные контрактов (%d) ---\n", worker.contracts_count);
    for (int i = 0; i < worker.contracts_count; i++) {
        printf("\nКонтракт #%d:\n", i + 1);
        printContract(worker.contracts[i]);
    }
    printf("===========================\n");
}

void freeWorker(Worker* worker) {
    if (worker == NULL) return;

    if (worker->full_name != NULL) {
        free(worker->full_name);
        worker->full_name = NULL;
    }

    freeContracts(worker);
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
    data->workers_capacity *= 2;
    Worker* temp = (Worker*)realloc(data->workers, data->workers_capacity * sizeof(Worker));
    if (temp == NULL) {
        printf("Ошибка перераспределения памяти\n");
        exit(EXIT_FAILURE);
    }
    data->workers = temp;
}

void addWorkerMenu(WorkersData* data) {
    if (data->workers_count >= data->workers_capacity) {
        increaseWorkersCapacity(data);
        if (data->workers == NULL) {
            printf("Ошибка: не удалось увеличить емкость массива работников!\n");
            return;
        }
    }

    Worker new_worker = createWorker(data->workers, data->workers_count);

    if (new_worker.full_name == NULL || new_worker.contracts_count == 0) {
        printf("Ошибка: не удалось создать работника!\n");
        freeWorker(&new_worker);
        return;
    }

    data->workers[data->workers_count] = new_worker;
    data->workers_count++;

    printf("\nРаботник %s успешно добавлен! Табельный номер: %d, контрактов: %d\n",
        new_worker.full_name, new_worker.personnel_number, new_worker.contracts_count);
}

int removeWorker(WorkersData* data, int index) {
    if (index < 0 || index >= data->workers_count || data->workers == NULL) {
        printf("Ошибка: неверный индекс работника\n");
        return 0;
    }

    freeWorker(&data->workers[index]);

    for (int i = index; i < data->workers_count - 1; i++) {
        data->workers[i] = data->workers[i + 1];
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
        printf("\nВведите индекс работника для удаления (1-%d) или 0 для отмены: ", data->workers_count);
        index = intchar();

        if (index == 0) {
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

PositionStats* createPositionsStats(WorkersData* data, int* stats_count) {
    *stats_count = 0;
    if (data->workers_count == 0) {
        return NULL;
    }

    PositionStats* stats = NULL;
    int stats_capacity = 10;
    stats = (PositionStats*)malloc(stats_capacity * sizeof(PositionStats));
    if (stats == NULL) {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }

    // Собираем статистику по всем контрактам всех работников
    for (int i = 0; i < data->workers_count; i++) {
        for (int k = 0; k < data->workers[i].contracts_count; k++) {
            char* current_pos = data->workers[i].contracts[k].position;
            int found = 0;

            for (int j = 0; j < *stats_count; j++) {
                if (strcmp(stats[j].position_name, current_pos) == 0) {
                    stats[j].avg_salary = (stats[j].avg_salary * stats[j].count +
                        data->workers[i].contracts[k].monthly_salary) /
                        (stats[j].count + 1);
                    stats[j].count++;
                    found = 1;
                    break;
                }
            }

            if (!found) {
                if (*stats_count >= stats_capacity) {
                    stats_capacity *= 2;
                    PositionStats* temp = (PositionStats*)realloc(stats,
                        stats_capacity * sizeof(PositionStats));
                    if (temp == NULL) {
                        printf("Ошибка перераспределения памяти!\n");
                        // Освобождаем уже выделенную память перед выходом
                        for (int m = 0; m < *stats_count; m++) {
                            free(stats[m].position_name);
                        }
                        free(stats);
                        return NULL;
                    }
                    stats = temp;
                }

                stats[*stats_count].position_name = _strdup(current_pos);
                if (stats[*stats_count].position_name == NULL) {
                    printf("Ошибка выделения памяти для названия должности!\n");
                    // Освобождаем уже выделенную память перед выходом
                    for (int m = 0; m < *stats_count; m++) {
                        free(stats[m].position_name);
                    }
                    free(stats);
                    return NULL;
                }
                stats[*stats_count].avg_salary = data->workers[i].contracts[k].monthly_salary;
                stats[*stats_count].count = 1;
                (*stats_count)++;
            }
        }
    }

    qsort(stats, *stats_count, sizeof(PositionStats), comparePositions);
    return stats;
}

void printPositionsStats(PositionStats* stats, int stats_count) {
    if (stats == NULL || stats_count == 0) {
        printf("Нет данных для отображения!\n");
        return;
    }

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
}

void positionsMenu(WorkersData* data) {
    if (data->workers_count == 0) {
        printf("Нет данных о работниках!\n");
        return;
    }

    int stats_count = 0;
    PositionStats* stats = createPositionsStats(data, &stats_count);
    if (stats != NULL) {
        printPositionsStats(stats, stats_count);

        // Освобождаем память
        for (int i = 0; i < stats_count; i++) {
            free(stats[i].position_name);
        }
        free(stats);
    }
}

void showMenu() {
    printf("\n1. Добавить работника\n");
    printf("2. Удалить работника\n");
    printf("3. Показать всех работников\n");
    printf("4. Топ должностей по зарплате\n");
    printf("5. Удалить уволенных работников\n");
    printf("6. Сохранить данные в файл\n");
    printf("7. Загрузить данные из файла\n");
    printf("0. Выход\n");
    printf("Выбор: ");
}

char* trimWhitespace(char* str) {
    if (str == NULL) return NULL;

    char* end;

    // Удаляем пробелы в начале
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) return str;

    // Удаляем пробелы в конце
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';

    return str;
}

void saveWorkersToFile(WorkersData* data, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла для записи!\n");
        return;
    }

    for (int i = 0; i < data->workers_count; i++) {
        Worker* worker = &data->workers[i];

        // Записываем количество контрактов первым числом
        fprintf(file, "%d | %s | %d | %d | %d | %d | %d",
            worker->contracts_count,
            worker->full_name,
            worker->birth_date.day, worker->birth_date.month, worker->birth_date.year,
            worker->personnel_number,
            worker->employment_type);  // Сохраняем тип занятости

        // Записываем все контракты
        for (int j = 0; j < worker->contracts_count; j++) {
            Contract* contract = &worker->contracts[j];
            fprintf(file, " | %s | %.2f | %d | %d | %d | %d | %d | %d | %d",
                contract->position,
                contract->rate,
                contract->monthly_salary,
                contract->start_date.day, contract->start_date.month, contract->start_date.year,
                contract->end_date.day, contract->end_date.month, contract->end_date.year);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Данные успешно сохранены в файл %s\n", filename);
}

void loadWorkersFromFile(WorkersData* data, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Файл %s не найден или не может быть открыт!\n", filename);
        return;
    }

    // Очищаем текущие данные
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

    char line[4096]; // Большой буфер для чтения длинных строк с несколькими контрактами
    while (fgets(line, sizeof(line), file)) {
        if (data->workers_count >= data->workers_capacity) {
            data->workers_capacity *= 2;
            Worker* temp = (Worker*)realloc(data->workers, data->workers_capacity * sizeof(Worker));
            if (temp == NULL) {
                printf("Ошибка перераспределения памяти!\n");
                break;
            }
            data->workers = temp;
        }

        Worker* current = &data->workers[data->workers_count];
        current->full_name = NULL;
        initContracts(current);

        // Читаем количество контрактов
        int contracts_count;
        char* token = strtok(line, "|");
        if (token == NULL) continue;
        contracts_count = atoi(trimWhitespace(token));

        // Читаем основные данные работника
        token = strtok(NULL, "|");
        current->full_name = (char*)malloc(strlen(trimWhitespace(token)) + 1);
        strcpy(current->full_name, trimWhitespace(token));

        token = strtok(NULL, "|"); current->birth_date.day = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->birth_date.month = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->birth_date.year = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->personnel_number = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->employment_type = (EmploymentType)atoi(trimWhitespace(token));

        // Читаем контракты
        for (int i = 0; i < contracts_count; i++) {
            Contract contract;
            contract.position = NULL;

            token = strtok(NULL, "|");
            if (token == NULL) break;
            contract.position = (char*)malloc(strlen(trimWhitespace(token)) + 1);
            strcpy(contract.position, trimWhitespace(token));

            token = strtok(NULL, "|"); if (token == NULL) break; contract.rate = atof(trimWhitespace(token));
            token = strtok(NULL, "|"); if (token == NULL) break; contract.monthly_salary = atoi(trimWhitespace(token));
            token = strtok(NULL, "|"); if (token == NULL) break; contract.start_date.day = atoi(trimWhitespace(token));
            token = strtok(NULL, "|"); if (token == NULL) break; contract.start_date.month = atoi(trimWhitespace(token));
            token = strtok(NULL, "|"); if (token == NULL) break; contract.start_date.year = atoi(trimWhitespace(token));
            token = strtok(NULL, "|"); if (token == NULL) break; contract.end_date.day = atoi(trimWhitespace(token));
            token = strtok(NULL, "|"); if (token == NULL) break; contract.end_date.month = atoi(trimWhitespace(token));
            token = strtok(NULL, "|"); if (token == NULL) break; contract.end_date.year = atoi(trimWhitespace(token));

            addContractToWorker(current, contract);
        }

        data->workers_count++;
    }

    fclose(file);
    printf("Загружено %d работников из файла.\n", data->workers_count);
}

int isWorkerDismissed(Worker* worker, Date current_date) {
    // Если у работника нет контрактов, считаем его уволенным
    if (worker->contracts_count == 0) {
        return 1;
    }

    // Проверяем все контракты работника
    for (int i = 0; i < worker->contracts_count; i++) {
        // Если текущая дата находится в периоде действия хотя бы одного контракта,
        // работник не считается уволенным
        if (compareDates(current_date, worker->contracts[i].start_date) >= 0 &&
            compareDates(current_date, worker->contracts[i].end_date) <= 0) {
            return 0;
        }
    }
    // Если ни один контракт не покрывает текущую дату, работник уволен
    return 1;
}

void removeDismissedWorkers(WorkersData* data, Date current_date) {
    if (data->workers_count == 0) {
        printf("Нет работников для проверки!\n");
        return;
    }

    int removed_count = 0;

    for (int i = data->workers_count - 1; i >= 0; i--) {
        if (isWorkerDismissed(&data->workers[i], current_date)) {
            printf("Удаляем уволенного работника: %s\n", data->workers[i].full_name);
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

void workerStruct(const char argv) {
    int exit_submenu = 0;
    WorkersData workers_data;
    initWorkersData(&workers_data);

    do {
        showMenu();
        int answer = intchar();

        switch (answer) {
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

        /*if (!exit_submenu) {
            printf("\nНажмите любую клавишу для продолжения...");
            _getch();
        }*/
    } while (!exit_submenu);

    freeWorkersData(&workers_data);
}