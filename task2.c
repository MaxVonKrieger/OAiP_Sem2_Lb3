#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "task2.h"

Worker* workers = NULL;
int workers_count = 0;
int workers_capacity = 10;

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

void printDate(const Date date)
{
    // Выводит дату в формате "ДД.ММ.ГГГГ" (с ведущими нулями)
    printf("[Дата: %02d.%02d.%04d]", date.day, date.month, date.year);
}
//COMPARE
int compareDates(Date date1, Date date2) {
    // Сначала сравниваем годы
    if (date1.year > date2.year) return 1;
    if (date1.year < date2.year) return -1;

    // Если годы равны, сравниваем месяцы
    if (date1.month > date2.month) return 1;
    if (date1.month < date2.month) return -1;

    // Если месяцы равны, сравниваем дни
    if (date1.day > date2.day) return 1;
    if (date1.day < date2.day) return -1;

    // Если все поля равны
    return 0;
} //C
//CREATE
Date createDate()
{
    Date result;

    // Ввод дня
    do {
        printf("\nВведите день (от 01 до 31): ");
        result.day = intchar();
    } while (result.day < 1 || result.day > 31);

    // Ввод месяца
    do {
        printf("\nВведите месяц (от 01 до 12): ");
        result.month = intchar();
    } while (result.month < 1 || result.month > 12);

    // Ввод года
    do {
        printf("\nВведите год(____): ");
        result.year = intchar();
    } while (result.year < 1900 || result.year > 2100);

    printf("\n");

    return result;  // Добавлен возврат структуры
}

Contract createContract() {
    Contract result;

    // Ввод должности (динамическая строка)
    printf("\nВведите должность: ");
    InsertStr(&result.position);

    // Ввод ставки
    do {
        printf("\nВведите ставку (0.1-10.0): ");
        printf("\n");
        result.rate = floatchar();  // Предполагается функция для ввода float
    } while (result.rate < 0.1f || result.rate > 10.0f);

    // Ввод зарплаты
    do {
        printf("\nВведите ежемесячную зарплату (> 0): ");
        result.monthly_salary = intchar();
    } while (result.monthly_salary <= 0);

    // Ввод даты начала
    printf("\nДата начала контракта:");
    result.start_date = createDate();

    // Ввод дата окончания (с проверкой что позже даты начала)
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

    // Освобождаем ФИО
    if (worker->full_name != NULL) {
        free(worker->full_name);
        worker->full_name = NULL;
    }

    // Освобождаем должность в контракте
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

Worker createWorker() {
    Worker result;
    result.full_name = NULL; // Явная инициализация
    result.contract.position = NULL; // Инициализация указателя на ФИО

    // Ввод ФИО
    printf("\nВведите ФИО работника: ");
    InsertStr(&result.full_name);

    // Ввод даты рождения
    printf("\nДата рождения работника:\n");
    result.birth_date = createDate();

    // Ввод табельного номера
    do {
        printf("\nВведите табельный номер (уникальное число > 0): ");
        result.personnel_number = intchar();
    } while (result.personnel_number <= 0);

    // Ввод данных контракта
    printf("\nВвод данных контракта:\n");
    result.contract = createContract(); // Создаем одиночный контракт

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
    printf("Должность: %s\n", worker.contract.position);
    printf("Ставка: %.2f\n", worker.contract.rate);
    printf("Зарплата: %d руб.\n", worker.contract.monthly_salary);

    printf("Дата начала: ");
    printDate(worker.contract.start_date);
    printf("\n");

    printf("Дата окончания: ");
    printDate(worker.contract.end_date);
    printf("\n");
    printf("===========================\n");
}
int isWorkersFull()
{
    return workers_capacity - workers_count;
}
void increaseWorkers()
{
    workers = realloc(workers, (workers_capacity + 1) * sizeof(Worker));
}

// Добавление нового работника
void addWorkerMenu() {
    // Проверка и увеличение массива при необходимости
    if (isWorkersFull()) {
        increaseWorkers();
    }

    // Создаем нового работника
    Worker new_worker = createWorker();

    // Добавляем в массив
    workers[workers_count] = new_worker;
    workers_count++;

    printf("\nРаботник успешно добавлен!\n");
}


// Функция удаления работника по индексу с полным освобождением памяти
int removeWorker(int index) {
    // Проверка валидности индекса
    if (index < 0 || index >= workers_count || workers == NULL) {
        printf("Ошибка: неверный индекс работника\n");
        return 0; // Неудача
    }

    // 1. Освобождаем память удаляемого работника
    freeWorker(&workers[index]);

    // 2. Сдвигаем остальные элементы массива
    int i;
    for (int i = index; i < workers_count - 1; i++) {
        freeWorker(&workers[i]);  // Освобождаем старые данные
        workers[i] = workers[i + 1];  // Копируем
        workers[i + 1].full_name = NULL;  // Обнуляем указатели у следующего
        workers[i + 1].contract.position = NULL;
    }

    // 3. Уменьшаем счетчик работников
    workers_count--;

    //если всё сломается, то удаляй это
   // freeWorker(&workers[i]);

    return 1; // Успех
}
void printAllWorkers() {
    if (workers_count == 0) {
        printf("\n=== Нет данных о работниках ===\n");
        return;
    }

    printf("\n=== СПИСОК РАБОТНИКОВ (%d) ===\n", workers_count);

    for (int i = 0; i < workers_count; i++) {
        printf("\n[Работник #%d]\n", i + 1);
        printWorker(workers[i]);
    }

    printf("\n=== Конец списка ===\n");

}
void removeWorkerMenu() {
    if (workers_count == 0) {
        printf("\nСписок работников пуст!\n");
        return;
    }

    // Выводим список работников для наглядности
    printf("\nТекущий список работников:\n");
    for (int i = 0; i < workers_count; i++) {
        printAllWorkers();
    }

    // Запрашиваем индекс
    int index;
    do {
        printf("\nВведите индекс работника для удаления (0-%d) или -1 для отмены: ", workers_count);
        index = intchar();

        if (index == -1) {
            printf("Удаление отменено.\n");
            return;
        }

        if (index < 0 || index > workers_count) {
            printf("Ошибка: неверный индекс! Попробуйте снова.\n");
        }
    } while (index < 0 || index > workers_count);

    // Подтверждение с использованием intchar()
    printf("\nВы действительно хотите удалить работника %s? (1 - да, 0 - нет): ",
        workers[index - 1].full_name);

    int confirm;
    do {
        confirm = intchar();
        if (confirm != 0 && confirm != 1) {
            printf("Введите 1 для подтверждения или 0 для отмены: ");
        }
    } while (confirm != 0 && confirm != 1);

    if (confirm == 1) {
        if (removeWorker(index - 1)) {
            printf("\nРаботник успешно удален!\n");
            printf("Осталось работников: %d\n", workers_count);
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

// Функция вывода всех должностей с статистикой
void printAllPositions() {
    if (workers_count == 0) {
        printf("Нет данных о работниках!\n");
        return;
    }

    // Создаем временный массив для статистики
    PositionStats* stats = NULL;
    int stats_count = 0;
    int stats_capacity = 10;
    stats = (PositionStats*)malloc(stats_capacity * sizeof(PositionStats));
    if (stats == NULL) {
        printf("Ошибка выделения памяти!\n");
        return;
    }

    // Собираем статистику по должностям
    for (int i = 0; i < workers_count; i++) {
        char* current_pos = workers[i].contract.position;
        int found = 0;

        // Ищем должность в статистике
        for (int j = 0; j < stats_count; j++) {
            if (strcmp(stats[j].position_name, current_pos) == 0) {
                stats[j].avg_salary = (stats[j].avg_salary * stats[j].count +
                    workers[i].contract.monthly_salary) /
                    (stats[j].count + 1);
                stats[j].count++;
                found = 1;
                break;
            }
        }

        // Если должность новая - добавляем
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
            stats[stats_count].avg_salary = workers[i].contract.monthly_salary;
            stats[stats_count].count = 1;
            stats_count++;
        }
    }

    // Сортируем по средней зарплате (по убыванию)
    qsort(stats, stats_count, sizeof(PositionStats), comparePositions);

    // Выводим все должности
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

    // Освобождаем память
    for (int i = 0; i < stats_count; i++) {
        free(stats[i].position_name);
    }
    free(stats);
}

// Функция для меню (заменяем предыдущую)
void positionsMenu() {
    if (workers_count == 0) {
        printf("Нет данных о работниках!\n");
        return;
    }

    printAllPositions();
}

void showMenu() {
    printf("\n1. Добавить\n2. Удалить\n3. Список\n4. Топ\n5. Удалить уволенных\n6. Сохранить в файл\n7. Загрузить из файла\n0. Выход\nВыбор: ");
}
// Функция освобождения памяти одного работника

void saveWorkersToFile(const char* argv[]) {
    FILE* file = fopen(argv, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла для записи!\n");
        return;
    }

    for (int i = 0; i < workers_count; i++) {
        Worker* worker = &workers[i];
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
    printf("Данные успешно сохранены в файл workers.txt\n");
}

// Объявление вспомогательной функции (добавьте в начало файла)
char* trimWhitespace(char* str) {
    if (str == NULL) return NULL;

    char* end;

    // Удаляем пробелы в начале
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) return str; // Все было пробелами

    // Удаляем пробелы в конце
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';

    return str;
}

void loadWorkersFromFile(const char* argv[]) {
    FILE* file = fopen(argv, "r");
    if (file == NULL) {
        printf("Файл workers.txt не найден или не может быть открыт!\n");
        return;
    }

    // Очищаем текущие данные
    for (int i = 0; i < workers_count; i++) {
        freeWorker(&workers[i]);
    }
    free(workers);
    workers = NULL;
    workers_count = 0;
    workers_capacity = 10;
    workers = (Worker*)malloc(workers_capacity * sizeof(Worker));
    if (workers == NULL) {
        printf("Ошибка выделения памяти!\n");
        fclose(file);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (workers_count >= workers_capacity) {
            ++workers_capacity;
            Worker* temp = (Worker*)realloc(workers, workers_capacity * sizeof(Worker));
            if (temp == NULL) {
                printf("Ошибка перераспределения памяти!\n");
                break;
            }
            workers = temp;
        }

        Worker* current = &workers[workers_count];
        current->full_name = NULL;
        current->contract.position = NULL;

        // Парсинг строки
        char* token = strtok(line, "|");
        if (token == NULL) continue;

        // ФИО
        token = trimWhitespace(token);
        current->full_name = (char*)malloc(strlen(token) + 1);
        strcpy(current->full_name, token);

        // Дата рождения
        token = strtok(NULL, "|"); current->birth_date.day = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->birth_date.month = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->birth_date.year = atoi(trimWhitespace(token));

        // Табельный номер
        token = strtok(NULL, "|"); current->personnel_number = atoi(trimWhitespace(token));

        // Должность
        token = strtok(NULL, "|");
        token = trimWhitespace(token);
        current->contract.position = (char*)malloc(strlen(token) + 1);
        strcpy(current->contract.position, token);

        // Остальные поля (ставка, зарплата, даты)
        token = strtok(NULL, "|"); current->contract.rate = atof(trimWhitespace(token));
        token = strtok(NULL, "|"); current->contract.monthly_salary = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->contract.start_date.day = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->contract.start_date.month = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->contract.start_date.year = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->contract.end_date.day = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->contract.end_date.month = atoi(trimWhitespace(token));
        token = strtok(NULL, "|"); current->contract.end_date.year = atoi(trimWhitespace(token));

        workers_count++;
    }

    fclose(file);
    printf("Загружено %d работников из файла.\n", workers_count);
}

// Функция для удаления уволенных работников на указанную дату
void removeDismissedWorkers(Date current_date) {
    if (workers_count == 0) {
        printf("Нет работников для проверки!\n");
        return;
    }

    int removed_count = 0;

    // Проходим по всем работникам в обратном порядке (чтобы избежать проблем при удалении)
    for (int i = workers_count - 1; i >= 0; i--) {
        // Сравниваем дату окончания контракта с текущей датой
        if (compareDates(workers[i].contract.end_date, current_date) < 0) {
            // Если дата окончания контракта раньше текущей даты - удаляем работника
            printf("Удаляем уволенного работника: %s (контракт закончился %02d.%02d.%04d)\n",
                workers[i].full_name,
                workers[i].contract.end_date.day,
                workers[i].contract.end_date.month,
                workers[i].contract.end_date.year);

            if (removeWorker(i)) {
                removed_count++;
            }
        }
    }

    if (removed_count == 0) {
        printf("Нет уволенных работников на указанную дату.\n");
    }
    else {
        printf("Удалено %d уволенных работников. Осталось %d работников.\n",
            removed_count, workers_count);
    }
}

// Функция для меню удаления уволенных работников
void removeDismissedMenu() {
    if (workers_count == 0) {
        printf("Нет работников для проверки!\n");
        return;
    }

    printf("\n=== Удаление уволенных работников ===\n");
    printf("Введите текущую дату для проверки:\n");
    Date current_date = createDate();

    removeDismissedWorkers(current_date);
}


void workerStruct(const char argv)
{
    do
    {
        int exit_submenu = 0;
        showMenu();
        int answer = intchar();
        switch (answer)
        {
        case 1:
            addWorkerMenu();
            break;
        case 2:
            removeWorkerMenu();
            break;
        case 3:
            printAllWorkers();
            break;
        case 4:
            positionsMenu();
            break;
        case 5:
            removeDismissedMenu();
            break;
        case 6:
            saveWorkersToFile(argv);
            break;
        case 7:
            loadWorkersFromFile(argv);
            break;
        case 0:
            exit_submenu = 1; // Устанавливаем флаг выхода
            break;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
            break;
        }
    } while (!exit_submenu);
}
