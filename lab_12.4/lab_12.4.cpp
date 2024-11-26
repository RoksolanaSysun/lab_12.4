#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <windows.h>

using namespace std;

struct Note {
    char surname[50];
    char name[50];
    char phone[20];
    int birth_date[3]; // 0: day, 1: month, 2: year
};

void Create(const string& file_name);
void Print(const string& file_name);
void SortNotes(const string& file_name);
void FindByMonth(const string& file_name);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string file_name;

    cout << "Введіть ім'я файлу: ";
    cin >> file_name;

    int menu_item;
    while (true) {
        cout << "\nОберіть дію:\n"
            << "1 - Ввести дані\n"
            << "2 - Вивести дані\n"
            << "3 - Сортувати за алфавітом\n"
            << "4 - Знайти за місяцем народження\n"
            << "0 - Завершити програму\n"
            << "Введіть: ";
        cin >> menu_item;

        switch (menu_item) {
        case 1: Create(file_name); break;
        case 2: Print(file_name); break;
        case 3: SortNotes(file_name); break;
        case 4: FindByMonth(file_name); break;
        case 0: return 0;
        default: cout << "Некоректний вибір. Спробуйте ще раз.\n";
        }
    }
}

void Create(const string& file_name) {
    ofstream f(file_name, ios::out | ios::binary);
    if (!f.is_open()) {
        cout << "Не вдалося відкрити файл для запису!" << endl;
        return;
    }

    Note note;
    char choice;
    do {
        cout << "Введіть прізвище: ";
        cin.ignore();
        cin.getline(note.surname, 50);
        cout << "Введіть ім'я: ";
        cin.getline(note.name, 50);
        cout << "Введіть номер телефону: ";
        cin.getline(note.phone, 20);
        cout << "Введіть день народження: ";
        cin >> note.birth_date[0];
        cout << "Введіть місяць народження: ";
        cin >> note.birth_date[1];
        cout << "Введіть рік народження: ";
        cin >> note.birth_date[2];

        f.write(reinterpret_cast<char*>(&note), sizeof(Note));

        cout << "Бажаєте ввести ще одну нотатку? (Y/N): ";
        cin >> choice;
    } while (choice == 'Y' || choice == 'y');

    f.close();
}

void Print(const string& file_name) {
    ifstream f(file_name, ios::in | ios::binary);
    if (!f.is_open()) {
        cout << "Не вдалося відкрити файл для читання!" << endl;
        return;
    }

    Note note;
    cout << "\n====================================================================\n";
    cout << "| Прізвище       | Ім'я           | Телефон       | Дата народження |\n";
    cout << "--------------------------------------------------------------------\n";

    while (f.read(reinterpret_cast<char*>(&note), sizeof(Note))) {
        cout << "| " << setw(14) << note.surname
            << " | " << setw(14) << note.name
            << " | " << setw(12) << note.phone
            << "  | " << setw(2) << note.birth_date[0] << "."
            << setw(2) << note.birth_date[1] << "."
            << setw(4) << note.birth_date[2] << "      |\n";
    }

    cout << "====================================================================\n";
    f.close();
}

void SortNotes(const string& file_name) {
    ifstream f(file_name, ios::in | ios::binary);
    if (!f.is_open()) {
        cout << "Не вдалося відкрити файл для читання!" << endl;
        return;
    }

    // Створення масиву записів
    Note notes[100];
    int count = 0;

    while (f.read(reinterpret_cast<char*>(&notes[count]), sizeof(Note))) {
        count++;
    }
    f.close();

    // Сортування
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(notes[j].surname, notes[j + 1].surname) > 0) {
                swap(notes[j], notes[j + 1]);
            }
        }
    }

    ofstream out_file(file_name, ios::out | ios::binary);
    if (!out_file.is_open()) {
        cout << "Не вдалося відкрити файл для запису!" << endl;
        return;
    }

    for (int i = 0; i < count; i++) {
        out_file.write(reinterpret_cast<char*>(&notes[i]), sizeof(Note));
    }

    out_file.close();
    cout << "Записи відсортовано за алфавітом.\n";
}

void FindByMonth(const string& file_name) {
    ifstream f(file_name, ios::in | ios::binary);
    if (!f.is_open()) {
        cout << "Не вдалося відкрити файл для читання!" << endl;
        return;
    }

    int month;
    cout << "Введіть номер місяця для пошуку: ";
    cin >> month;

    Note note;
    bool found = false;

    cout << "\n====================================================================\n";
    cout << "| Прізвище       | Ім'я           | Телефон       | Дата народження |\n";
    cout << "--------------------------------------------------------------------\n";

    while (f.read(reinterpret_cast<char*>(&note), sizeof(Note))) {
        if (note.birth_date[1] == month) {
            cout << "| " << setw(14) << note.surname
                << " | " << setw(14) << note.name
                << " | " << setw(12) << note.phone
                << "  | " << setw(2) << note.birth_date[0] << "."
                << setw(2) << note.birth_date[1] << "."
                << setw(4) << note.birth_date[2] << "      |\n";
            found = true;
        }
    }

    if (!found) {
        cout << "|                  Немає записів для цього місяця             |\n";
    }

    cout << "====================================================================\n";

    f.close();
}

