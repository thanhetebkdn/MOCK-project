#include "view/MetadataView.h"
#include "common/Exception.h"
#include <iostream>
#include <climits>

// void MetadataView::showMenu() {
//     cout << "\n+---------------------------------+" << endl;
//     cout << "|       Edit Metadata Menu        |" << endl;
//     cout << "+---------------------------------+" << endl;
//     cout << "| 1. Edit Title                   |" << endl;
//     cout << "| 2. Edit Artist                  |" << endl;
//     cout << "| 3. Edit Album                   |" << endl;
//     cout << "| 4. Edit Genre                   |" << endl;
//     cout << "| 5. Edit Year                    |" << endl;
//     cout << "| 6. Edit Track                   |" << endl;
//     cout << "| 0. Exit Editing                 |" << endl;
//     cout << "+---------------------------------+" << endl;
//     cout << "Enter your choice: ";
// }



// int MetadataView::handleInput() {
//     int choice = Exception::getValidatedInput_Int("Choose an option: ", Exception::validateInputMetadataView);
//     return choice;
// }
int MetadataView::showMenu() {
    MenuRenderer menu("Edit Metadata Menu", 
                      {"1. Edit Title", "2. Edit Artist", "3. Edit Album", 
                       "4. Edit Genre", "5. Edit Year", "6. Edit Track", "0. Exit Editing"},
                      {1, 2, 3, 4, 5, 6, 0});
    return menu.render();
}



// void MetadataView::displayMetadata(const map<string, string>& metadata) {
//     const int fieldWidth = 20;   // Chiều rộng của cột "Field"
//     const int valueWidth = 40;  // Chiều rộng của cột "Value"
//     const int totalWidth = fieldWidth + valueWidth + 5; // Tổng chiều rộng: cột Field + cột Value + khoảng cách

//     // Hàm vẽ dòng ngang
//     auto drawLine = [&]() {
//         cout << "+" << string(fieldWidth + 1, '-') 
//                   << "+" << string(valueWidth + 1, '-') 
//                   << "+" << endl;
//     };

//     // Hiển thị header bảng
//     drawLine();
//     cout << "| " << setw(fieldWidth) << left << "Field"
//               << "| " << setw(valueWidth) << left << "Value"
//               << " |" << endl;
//     drawLine();

//     // Hiển thị nội dung metadata
//     for (const auto& field : metadata) {
//         string key = field.first;
//         string value = field.second;

//         // Cắt chuỗi giá trị nếu quá dài
//         while (value.length() > valueWidth) {
//             // In dòng đầu tiên với key
//             cout << "| " << setw(fieldWidth) << left << key
//                       << "| " << setw(valueWidth) << left << value.substr(0, valueWidth)
//                       << " |" << endl;

//             // Loại bỏ phần đã in và thay key bằng chuỗi trống
//             value = value.substr(valueWidth);
//             key = ""; // Key chỉ in ở dòng đầu tiên
//         }

//         // In dòng cuối cùng hoặc chuỗi ngắn
//         cout << "| " << setw(fieldWidth) << left << key
//                   << "| " << setw(valueWidth) << left << value
//                   << " |" << endl;
//     }

//     // Hiển thị footer bảng
//     drawLine();
// }
void MetadataView::displayMetadata(const map<string, string>& metadata) {
    const int fieldWidth = 20;   // Chiều rộng của cột "Field"
    const int valueWidth = 40;  // Chiều rộng của cột "Value"
    const int totalWidth = fieldWidth + valueWidth + 5; // Tổng chiều rộng: cột Field + cột Value + khoảng cách

    // Hàm vẽ dòng ngang
    auto drawLine = [&]() {
        cout << "+" << string(fieldWidth + 1, '-') 
                  << "+" << string(valueWidth + 1, '-') 
                  << "+" << endl;
    };

    // Hiển thị header bảng
    drawLine();
    cout << "| " << "Field" 
              << string(fieldWidth - 5, ' ') // Thêm khoảng trắng cho cột "Field"
              << "| " << "Value" 
              << string(valueWidth - 5, ' ') // Thêm khoảng trắng cho cột "Value"
              << " |" << endl;
    drawLine();

    // Hiển thị nội dung metadata
    for (const auto& field : metadata) {
        string key = field.first;
        string value = field.second;

        // Cắt chuỗi giá trị nếu quá dài
        while (value.length() > valueWidth) {
            // In dòng đầu tiên với key
            cout << "| " << key;
            if (key.length() < fieldWidth) {
                cout << string(fieldWidth - key.length(), ' '); // Thêm khoảng trắng để căn chỉnh cột "Field"
            }
            cout << "| " << value.substr(0, valueWidth); // In một phần giá trị
            if (valueWidth > value.substr(0, valueWidth).length()) {
                cout << string(valueWidth - value.substr(0, valueWidth).length(), ' '); // Thêm khoảng trắng
            }
            cout << " |\n";

            // Loại bỏ phần đã in và thay key bằng chuỗi trống
            value = value.substr(valueWidth);
            key = ""; // Key chỉ in ở dòng đầu tiên
        }

        // In dòng cuối cùng hoặc chuỗi ngắn
        cout << "| " << key;
        if (key.length() < fieldWidth) {
            cout << string(fieldWidth - key.length(), ' '); // Thêm khoảng trắng để căn chỉnh cột "Field"
        }
        cout << "| " << value;
        if (value.length() < valueWidth) {
            cout << string(valueWidth - value.length(), ' '); // Thêm khoảng trắng
        }
        cout << " |\n";
    }

    // Hiển thị footer bảng
    drawLine();
}