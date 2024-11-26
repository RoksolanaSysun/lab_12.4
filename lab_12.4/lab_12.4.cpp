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

    cout << "������ ��'� �����: ";
    cin >> file_name;

    int menu_item;
    while (true) {
        cout << "\n������ ��:\n"
            << "1 - ������ ���\n"
            << "2 - ������� ���\n"
            << "3 - ��������� �� ��������\n"
            << "4 - ������ �� ������ ����������\n"
            << "0 - ��������� ��������\n"
            << "������: ";
        cin >> menu_item;

        switch (menu_item) {
        case 1: Create(file_name); break;
        case 2: Print(file_name); break;
        case 3: SortNotes(file_name); break;
        case 4: FindByMonth(file_name); break;
        case 0: return 0;
        default: cout << "����������� ����. ��������� �� ���.\n";
        }
    }
}

void Create(const string& file_name) {
    ofstream f(file_name, ios::out | ios::binary);
    if (!f.is_open()) {
        cout << "�� ������� ������� ���� ��� ������!" << endl;
        return;
    }

    Note note;
    char choice;
    do {
        cout << "������ �������: ";
        cin.ignore();
        cin.getline(note.surname, 50);
        cout << "������ ��'�: ";
        cin.getline(note.name, 50);
        cout << "������ ����� ��������: ";
        cin.getline(note.phone, 20);
        cout << "������ ���� ����������: ";
        cin >> note.birth_date[0];
        cout << "������ ����� ����������: ";
        cin >> note.birth_date[1];
        cout << "������ �� ����������: ";
        cin >> note.birth_date[2];

        f.write(reinterpret_cast<char*>(&note), sizeof(Note));

        cout << "������ ������ �� ���� �������? (Y/N): ";
        cin >> choice;
    } while (choice == 'Y' || choice == 'y');

    f.close();
}

void Print(const string& file_name) {
    ifstream f(file_name, ios::in | ios::binary);
    if (!f.is_open()) {
        cout << "�� ������� ������� ���� ��� �������!" << endl;
        return;
    }

    Note note;
    cout << "\n====================================================================\n";
    cout << "| �������       | ��'�           | �������       | ���� ���������� |\n";
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
        cout << "�� ������� ������� ���� ��� �������!" << endl;
        return;
    }

    // ��������� ������ ������
    Note notes[100];
    int count = 0;

    while (f.read(reinterpret_cast<char*>(&notes[count]), sizeof(Note))) {
        count++;
    }
    f.close();

    // ����������
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(notes[j].surname, notes[j + 1].surname) > 0) {
                swap(notes[j], notes[j + 1]);
            }
        }
    }

    ofstream out_file(file_name, ios::out | ios::binary);
    if (!out_file.is_open()) {
        cout << "�� ������� ������� ���� ��� ������!" << endl;
        return;
    }

    for (int i = 0; i < count; i++) {
        out_file.write(reinterpret_cast<char*>(&notes[i]), sizeof(Note));
    }

    out_file.close();
    cout << "������ ����������� �� ��������.\n";
}

void FindByMonth(const string& file_name) {
    ifstream f(file_name, ios::in | ios::binary);
    if (!f.is_open()) {
        cout << "�� ������� ������� ���� ��� �������!" << endl;
        return;
    }

    int month;
    cout << "������ ����� ����� ��� ������: ";
    cin >> month;

    Note note;
    bool found = false;

    cout << "\n====================================================================\n";
    cout << "| �������       | ��'�           | �������       | ���� ���������� |\n";
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
        cout << "|                  ���� ������ ��� ����� �����             |\n";
    }

    cout << "====================================================================\n";

    f.close();
}

