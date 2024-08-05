#include <string>
#include <sstream>
#include <map>
#include <msclr/marshal_cppstd.h>
#include <algorithm>
#include <vector>
#include <codecvt> 

namespace firstgui {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Windows::Forms;
    using namespace System::Data;

    public ref class MainForm : public System::Windows::Forms::Form
    {
    public:
        MainForm(void)
        {
            InitializeComponent();
            AddColumnsToDataGridView(); // Add columns during initialization
        }

    protected:
        ~MainForm()
        {
            if (components)
            {
                delete components;
            }
        }
    private: System::Windows::Forms::Label^ label1;
    private: System::Windows::Forms::TextBox^ txtCode;
    private: System::Windows::Forms::Button^ btnTokenize;
    private: System::Windows::Forms::DataGridView^ dataGridView1;

    private:
        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->txtCode = (gcnew System::Windows::Forms::TextBox());
            this->btnTokenize = (gcnew System::Windows::Forms::Button());
            this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(17, 13);
            this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(104, 16);
            this->label1->TabIndex = 0;
            this->label1->Text = L"Enter your code:";
            // 
            // txtCode
            // 
            this->txtCode->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14));
            this->txtCode->Location = System::Drawing::Point(21, 34);
            this->txtCode->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
            this->txtCode->Multiline = true;
            this->txtCode->Name = L"txtCode";
            this->txtCode->Size = System::Drawing::Size(1166, 212);
            this->txtCode->TabIndex = 1;
            // 
            // btnTokenize
            // 
            this->btnTokenize->Location = System::Drawing::Point(20, 252);
            this->btnTokenize->Name = L"btnTokenize";
            this->btnTokenize->Size = System::Drawing::Size(160, 42);
            this->btnTokenize->TabIndex = 2;
            this->btnTokenize->Text = L"Tokenize";
            this->btnTokenize->UseVisualStyleBackColor = true;
            this->btnTokenize->Click += gcnew System::EventHandler(this, &MainForm::btnTokenize_Click);
            // 
            // dataGridView1
            // 
            this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridView1->Location = System::Drawing::Point(21, 300);
            this->dataGridView1->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
            this->dataGridView1->Name = L"dataGridView1";
            this->dataGridView1->RowHeadersWidth = 100;
            this->dataGridView1->Size = System::Drawing::Size(1166, 380);
            this->dataGridView1->TabIndex = 3;
            // 
            // MainForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1500, 700);
            this->Controls->Add(this->dataGridView1);
            this->Controls->Add(this->btnTokenize);
            this->Controls->Add(this->txtCode);
            this->Controls->Add(this->label1);
            this->Margin = System::Windows::Forms::Padding(4, 3, 4, 3);
            this->Name = L"MainForm";
            this->Text = L"Tokenization Application";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    private:
        void AddColumnsToDataGridView() {
            // Add columns to DataGridView
            dataGridView1->Columns->Add("Token", "Token");
            dataGridView1->Columns->Add("Type", "Type");
            dataGridView1->Columns->Add("Syntax Error", "Syntax Error"); // New column for syntax error
            dataGridView1->Columns->Add("Line", "Line"); // New column for line number

            // Set column widths
            dataGridView1->Columns[0]->Width = 200;
            dataGridView1->Columns[1]->Width = 200;
            dataGridView1->Columns[2]->Width = 300;
            dataGridView1->Columns[3]->Width = 50; // Adjust width for line number column

            // Set column header style
            for (int i = 0; i < dataGridView1->Columns->Count; i++) {
                dataGridView1->Columns[i]->HeaderCell->Style->Font = gcnew System::Drawing::Font(dataGridView1->ColumnHeadersDefaultCellStyle->Font->FontFamily, 14, System::Drawing::FontStyle::Bold);
            }
        }

        std::tuple<std::string, std::string, int> classifyToken(const std::string& token, int lineNumber) {
            // Check if the token is a number
            bool isNumber = true;
            for (char c : token) {
                if (!isdigit(c)) {
                    isNumber = false;
                    break;
                }
            }

            if (isNumber) {
                return { "num : INTEGER", "", lineNumber };
            }

            // Check if the token matches any keyword
            if (token == "if" || token == "else" || token == "repeat" || token == "until" || token == "read" || token == "write" || token == "then" || token == "end") {
                return { "keyword : " + token, "", lineNumber };
            }

            // Check if the token is a string
            if (token.size() >= 2 && token.front() == '"' && token.back() == '"') {
                return { "STRING", "", lineNumber };
            }

            // Check if the token is an assignment operator :=
            if (token == ":=") {
                return { "ASSIGN", "", lineNumber };
            }

            // Check if the token is a single-character token
            if (token.size() == 1) {
                char singleChar = token.front();
                switch (singleChar) {
                case ';': return { "SEMICOLON", "", lineNumber };
                case '+': return { "PLUS", "", lineNumber };
                case '-': return { "MINUS", "", lineNumber };
                case '*': return { "MULTIPLY", "", lineNumber };
                case '/': return { "DIVIDE", "", lineNumber };
                case '<': return { "LT", "", lineNumber };
                case '=': return { "EQ", "", lineNumber };
                case '{': return { "LBRACE", "", lineNumber };
                case '}': return { "RBRACE", "", lineNumber };
                case ',': return { "COMMA", "", lineNumber };
                case '(': return { "LPUNCTUATION", "", lineNumber };
                case ')': return { "RPUNCTUATION", "", lineNumber };
                case '"': return { "quotes", "", lineNumber };
                default: break;
                }
            }

            // Check if the token is an identifier
            if (isalpha(token.front())) {
                return { "ID", "", lineNumber };
            }

            // If none of the above matched, it's an error
            return { "ERROR", "Unknown token", lineNumber };
        }



        bool IsPreviousTokenValid(String^ previousToken, String^ currentToken) {
            // Check if the previous token is an identifier and the current token is :=
            if (previousToken == "ID") {
                return true;
            }
            return false;
        }

        System::Void btnTokenize_Click(System::Object^ sender, System::EventArgs^ e) {
            // Tokenize the code and display in DataGridView
            String^ inputCode = this->txtCode->Text;
            std::wstring wstr = msclr::interop::marshal_as<std::wstring>(inputCode);
            std::wistringstream iss(wstr); // Use wistringstream to handle wide characters
            dataGridView1->Rows->Clear();
            int lineNumber = 1; // Track the line number

            // Draw DataGridView with tokens
            std::wstring line;
            while (std::getline(iss, line)) {
                std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
                std::string utf8_line = converter.to_bytes(line);
                std::istringstream line_iss(utf8_line);
                std::string token;
                bool ifFound = false; // Flag to track if an "if" statement has been found
                bool thenFound = false; // Flag to track if a "then" statement has been found
                bool repeatFound = false; // Flag to track if a "repeat" statement has been found
                bool untilFound = false; // Flag to track if an "until" statement has been found
                bool endfound = false;
                bool elsefound = false;
                bool semicolumnfound = false;
                // Process each token in the line
                while (line_iss >> token) {
                    auto classification = classifyToken(token, lineNumber);
                    std::string type = std::get<0>(classification);
                    std::string syntaxError = std::get<1>(classification);
                    DataGridViewRow^ row = gcnew DataGridViewRow();
                    row->CreateCells(dataGridView1);
                    row->Cells[0]->Value = gcnew String(token.c_str());
                    row->Cells[1]->Value = gcnew String(type.c_str());
                    row->Cells[3]->Value = lineNumber.ToString(); // Add line number to the DataGridView

                    // Add syntax error information
                    if (syntaxError != "") {
                        row->Cells[2]->Value = gcnew String(syntaxError.c_str());
                        row->Cells[2]->Style->BackColor = System::Drawing::Color::Red; // Highlight syntax error in red
                    }

                    dataGridView1->Rows->Add(row);

                    // Check for loop keywords
                    if (token == "if") {
                        ifFound = true;
                    }
                    else if (token == "then") {
                        if (!ifFound) {
                            // "then" found without preceding "if", add error row
                            DataGridViewRow^ errorRow = gcnew DataGridViewRow();
                            errorRow->CreateCells(dataGridView1);
                            errorRow->Cells[3]->Value = lineNumber.ToString();
                            dataGridView1->Rows->Add(errorRow);
                            errorRow->Cells[2]->Value = gcnew String("Misplaced 'then'");
                            errorRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                        }
                        else {
                            thenFound = true;
                        }
                    }
                    else if (token == "repeat") {
                        repeatFound = true;
                    }
                    else if (token == "until") {
                        if (!repeatFound) {
                            // "until" found without preceding "repeat", add error row
                            DataGridViewRow^ errorRow = gcnew DataGridViewRow();
                            errorRow->CreateCells(dataGridView1);
                            errorRow->Cells[3]->Value = lineNumber.ToString();
                            dataGridView1->Rows->Add(errorRow);
                            errorRow->Cells[2]->Value = gcnew String("Misplaced 'until'");
                            errorRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                        }
                        else {
                            untilFound = true;
                        }
                    }
                    else if (token == "end") {
                        if (!thenFound) {
                            DataGridViewRow^ errorRow = gcnew DataGridViewRow();
                            errorRow->CreateCells(dataGridView1);
                            errorRow->Cells[3]->Value = lineNumber.ToString();
                            dataGridView1->Rows->Add(errorRow);
                            errorRow->Cells[2]->Value = gcnew String("Misplaced 'end'");
                            errorRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                        }
                        endfound = true;
                    }
                    if (token == "else") {
                        if (!thenFound) {
                            DataGridViewRow^ errorRow = gcnew DataGridViewRow();
                            errorRow->CreateCells(dataGridView1);
                            errorRow->Cells[3]->Value = lineNumber.ToString();
                            dataGridView1->Rows->Add(errorRow);
                            errorRow->Cells[2]->Value = gcnew String("Misplaced 'else'");
                            errorRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                        }
                        endfound = true;
                    }
                    if (token == ";") {
                        semicolumnfound = true;
                    }
                }






                // Check for missing loop terminators
                if ((ifFound && !thenFound) || (repeatFound && !untilFound) || (thenFound && !endfound)) {
                    DataGridViewRow^ errorRow = gcnew DataGridViewRow();
                    errorRow->CreateCells(dataGridView1);
                    errorRow->Cells[3]->Value = lineNumber.ToString();
                    dataGridView1->Rows->Add(errorRow);
                    errorRow->Cells[2]->Value = gcnew String("Missing loop terminator");
                    errorRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                }

                if (endfound && !semicolumnfound) {
                    DataGridViewRow^ errorRow = gcnew DataGridViewRow();
                    errorRow->CreateCells(dataGridView1);
                    errorRow->Cells[3]->Value = lineNumber.ToString();
                    dataGridView1->Rows->Add(errorRow);
                    errorRow->Cells[2]->Value = gcnew String("semicolumn expected after 'end' keyword");
                    errorRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                }





                // Check for unmatched double quotes
                int quoteCount = 0;
                for (wchar_t c : line) {
                    if (c == L'"') {
                        quoteCount++;
                    }
                }
                if (quoteCount % 2 != 0) {
                    // Create a new row to indicate the missing quote
                    DataGridViewRow^ lastRow = gcnew DataGridViewRow();
                    lastRow->CreateCells(dataGridView1);
                    lastRow->Cells[3]->Value = lineNumber.ToString();
                    dataGridView1->Rows->Add(lastRow);
                    lastRow->Cells[2]->Value = gcnew String("Unmatched double quote");
                    lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                }

                lineNumber++; // Move to the next line
            }





            // Check if operator tokens are followed by correct tokens
            for (int i = 0; i < dataGridView1->Rows->Count - 1; ++i) {
                DataGridViewRow^ currentRow = dataGridView1->Rows[i];
                String^ currentToken = safe_cast<String^>(currentRow->Cells[0]->Value);
                String^ reached = "";

                if (IsKeyword(currentToken) && i + 1 < dataGridView1->Rows->Count) {
                    DataGridViewRow^ nextRow = dataGridView1->Rows[i + 1];
                    String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);
                    if (IsOperatorToken(nextToken) || currentToken == ";") {
                        DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                        lastRow->CreateCells(dataGridView1); // Create cells for the new row
                        lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                        dataGridView1->Rows->Add(lastRow); // Add the new row
                        lastRow->Cells[2]->Value = gcnew String("unexpected operator after keyword");
                        lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red; // Highlight s

                    }

                }





                // Check if the token is an assignment operator :=
                if (i - 1 >= 0 && currentToken == ":=") {
                    DataGridViewRow^ currentRow = dataGridView1->Rows[i];
                    DataGridViewRow^ previousRow = dataGridView1->Rows[i - 1];
                    String^ previousToken = safe_cast<String^>(previousRow->Cells[1]->Value);
                    if (!IsPreviousTokenValid(previousToken, currentToken)) {
                        DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                        lastRow->CreateCells(dataGridView1); // Create cells for the new row
                        lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                        dataGridView1->Rows->Add(lastRow); // Add the new row
                        lastRow->Cells[2]->Value = gcnew String("Identifier expected before ':='");
                        lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red; // Highlight s
                    }

                }



                if (i + 1 < dataGridView1->Rows->Count) {


                    if (dataGridView1->Rows[i + 1]->Cells[0]->Value == nullptr || dataGridView1->Rows[i + 1]->Cells[0]->Value->ToString() == "") {
                        if (currentToken == "if" || currentToken == "else" || currentToken == "repeat" || currentToken == "until" || currentToken == "read" || currentToken == "write") {

                            DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                            lastRow->CreateCells(dataGridView1); // Create cells for the new row
                            lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                            dataGridView1->Rows->Add(lastRow); // Add the new row
                            lastRow->Cells[2]->Value = gcnew String("No token after keyword");
                            lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red; // Highlight s
                        }
                        if (IsOperatorToken(currentToken)) {

                            DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                            lastRow->CreateCells(dataGridView1); // Create cells for the new row
                            lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                            dataGridView1->Rows->Add(lastRow); // Add the new row
                            lastRow->Cells[2]->Value = gcnew String("No token after operator");
                            lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                        }
                        continue;
                    }







                    if (currentToken == "if") {
                        reached = " ";
                        for (int w = i; w < dataGridView1->Rows->Count - 1; ++w) {

                            if (dataGridView1->Rows[w + 1]->Cells[0]->Value == nullptr || dataGridView1->Rows[w + 1]->Cells[0]->Value->ToString() == "") {

                                break;
                            }
                            DataGridViewRow^ nextRow = dataGridView1->Rows[w + 1];
                            String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);



                            if (!IsIdentifierToken(nextToken) && !IsStringDigit(nextToken)&& nextToken!="=") {
                                DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                dataGridView1->Rows->Add(lastRow); // Add the new row
                                lastRow->Cells[2]->Value = gcnew String("Wrong if expression");
                                lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                                break;
                            }

                            if (IsIdentifierToken(nextToken) || IsStringDigit(nextToken)) {
                                DataGridViewRow^ nextRow = dataGridView1->Rows[w + 2];
                                String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);
                                if (dataGridView1->Rows[w + 2]->Cells[0]->Value == nullptr || dataGridView1->Rows[w + 2]->Cells[0]->Value->ToString() == "") {
                                    DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                    lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                    lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                    dataGridView1->Rows->Add(lastRow); // Add the new row
                                    lastRow->Cells[2]->Value = gcnew String("Wrong if expression");
                                    lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                                    break;
                                }





                                else {
                                    DataGridViewRow^ nextRow = dataGridView1->Rows[w + 2];
                                    String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);
                                    if (nextToken == "then" && reached == "yes") {
                                        break;
                                    }

                                    if (nextToken == "+" || nextToken == "-" || nextToken == "*" || nextToken == "/") {


                                        if (dataGridView1->Rows[w + 3]->Cells[0]->Value == nullptr || dataGridView1->Rows[w + 3]->Cells[0]->Value->ToString() == "") {
                                            DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                            lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                            lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                            dataGridView1->Rows->Add(lastRow); // Add the new row
                                            lastRow->Cells[2]->Value = gcnew String("Wrong if expression");
                                            lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                                            break;
                                        }

                                        else {
                                            DataGridViewRow^ nextRow = dataGridView1->Rows[w + 3];
                                            String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);
                                            if (IsIdentifierToken(nextToken) || IsStringDigit(nextToken)) {
                                                w += 1;
                                                continue;
                                            }
                                            else {
                                                DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                                lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                                lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                                dataGridView1->Rows->Add(lastRow); // Add the new row
                                                lastRow->Cells[2]->Value = gcnew String("Wrong if expression");
                                                lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                                                break;
                                            }

                                        }
                                    }


                                    else  if ((nextToken == "=" || nextToken == "<") && reached != "yes") {


                                        if (dataGridView1->Rows[w + 3]->Cells[0]->Value == nullptr || dataGridView1->Rows[w + 3]->Cells[0]->Value->ToString() == "") {
                                            DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                            lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                            lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                            dataGridView1->Rows->Add(lastRow); // Add the new row
                                            lastRow->Cells[2]->Value = gcnew String("Wrong if expression");
                                            lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                                            break;
                                        }


                                        DataGridViewRow^ nextRow = dataGridView1->Rows[w + 3];
                                        String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);
                                        if ((!IsIdentifierToken(nextToken) && !IsStringDigit(nextToken))) {
                                            DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                            lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                            lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                            dataGridView1->Rows->Add(lastRow); // Add the new row
                                            lastRow->Cells[2]->Value = gcnew String("Wrong if expression");
                                            lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                                            break;
                                        }


                                        else {


                                            reached = "yes";

                                            continue;







                                        }
                                    }






                                }
                            }
                        }




                    }





                    DataGridViewRow^ nextRow = dataGridView1->Rows[i + 1];
                    String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);

                    if (currentToken == ":=") {
                        reached = " ";
                        for (int w = i; w < dataGridView1->Rows->Count - 1; ++w) {
                            if (dataGridView1->Rows[w + 1]->Cells[0]->Value == nullptr || dataGridView1->Rows[w + 1]->Cells[0]->Value->ToString() == "") {

                                break;
                            }





                            DataGridViewRow^ nextRow = dataGridView1->Rows[w + 1];
                            String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);
                            if (!IsIdentifierToken(nextToken) && !IsStringDigit(nextToken)) {
                                break;
                            }

                            if (IsIdentifierToken(dataGridView1->Rows[w + 2]->Cells[0]->Value->ToString()) || IsKeyword(dataGridView1->Rows[w + 2]->Cells[0]->Value->ToString())) {
                                DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                dataGridView1->Rows->Add(lastRow); // Add the new row
                                lastRow->Cells[2]->Value = gcnew String("missing semicolumn at end of statment");
                                lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red; // Highlight s
                                break;
                            }

                            nextRow = dataGridView1->Rows[w + 2];
                            nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);

                            if (nextToken == "+" || nextToken == "-" || nextToken == "*" || nextToken == "/") {


                                if (dataGridView1->Rows[w + 3]->Cells[0]->Value == nullptr || dataGridView1->Rows[w + 3]->Cells[0]->Value->ToString() == "") {
                                    break;
                                }



                                else {
                                    DataGridViewRow^ nextRow = dataGridView1->Rows[w + 3];
                                    String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);
                                    if (IsIdentifierToken(nextToken) || IsStringDigit(nextToken)) {
                                        nextRow = dataGridView1->Rows[w + 4];
                                        nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);
                                        if (dataGridView1->Rows[w + 4]->Cells[0]->Value == nullptr || dataGridView1->Rows[w + 4]->Cells[0]->Value->ToString() == "" || dataGridView1->Rows[w + 4]->Cells[0]->Value->ToString() != ";") {
                                            DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                            lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                            lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                            dataGridView1->Rows->Add(lastRow); // Add the new row
                                            lastRow->Cells[2]->Value = gcnew String("missing semicolumn at end of assignment");
                                            lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red; // Highlight s
                                            break;
                                        }

                                        DataGridViewRow^ nextRow = dataGridView1->Rows[w + 4];
                                        String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);
                                        if (nextToken == "+" || nextToken == "-" || nextToken == "*" || nextToken == "/") {
                                            w += 1;

                                            continue;
                                        }
                                    }

                                }
                            }



                        }
                    }

                    if (currentToken == "then" || currentToken == "end") {
                        if ((dataGridView1->Rows[i + 1]->Cells[0]->Value == nullptr || dataGridView1->Rows[i + 1]->Cells[0]->Value->ToString() == "")) {
                            DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                            lastRow->CreateCells(dataGridView1); // Create cells for the new row
                            lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                            dataGridView1->Rows->Add(lastRow); // Add the new row
                            lastRow->Cells[2]->Value = gcnew String("no token after keyword");
                        }


                    }



                    else

                        if (currentToken == "until") {
                            reached = " ";
                            for (int w = i; w < dataGridView1->Rows->Count - 1; ++w) {

                                if (dataGridView1->Rows[w + 1]->Cells[0]->Value == nullptr || dataGridView1->Rows[w + 1]->Cells[0]->Value->ToString() == "") {

                                    break;
                                }
                                DataGridViewRow^ nextRow = dataGridView1->Rows[w + 1];
                                String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);



                                if (!IsIdentifierToken(nextToken) && !IsStringDigit(nextToken)&& nextToken !="=") {
                                    DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                    lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                    lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                    dataGridView1->Rows->Add(lastRow); // Add the new row
                                    lastRow->Cells[2]->Value = gcnew String("Wrong until expression");
                                    lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                                    break;
                                }

                                if (IsIdentifierToken(nextToken) || IsStringDigit(nextToken)) {

                                    if (dataGridView1->Rows[w + 2]->Cells[0]->Value == nullptr || dataGridView1->Rows[w + 2]->Cells[0]->Value->ToString() == "") {
                                        DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                        lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                        lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                        dataGridView1->Rows->Add(lastRow); // Add the new row
                                        lastRow->Cells[2]->Value = gcnew String("Wrong until expression");
                                        lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                                        break;
                                    }



                                    else {
                                        DataGridViewRow^ nextRow = dataGridView1->Rows[w + 2];
                                        String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);
                                        if (nextToken == ";" && reached == "yes") {
                                            break;
                                        }

                                        if (nextToken == "+" || nextToken == "-" || nextToken == "*" || nextToken == "/") {


                                            if (dataGridView1->Rows[w + 3]->Cells[0]->Value == nullptr || dataGridView1->Rows[w + 3]->Cells[0]->Value->ToString() == "") {
                                                DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                                lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                                lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                                dataGridView1->Rows->Add(lastRow); // Add the new row
                                                lastRow->Cells[2]->Value = gcnew String("Wrong until expression");
                                                lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                                                break;
                                            }

                                            else {
                                                DataGridViewRow^ nextRow = dataGridView1->Rows[w + 3];
                                                String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);
                                                if (IsIdentifierToken(nextToken) || IsStringDigit(nextToken)) {
                                                    w += 1;
                                                    continue;
                                                }
                                                else {
                                                    DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                                    lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                                    lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                                    dataGridView1->Rows->Add(lastRow); // Add the new row
                                                    lastRow->Cells[2]->Value = gcnew String("Wrong until expression");
                                                    lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                                                    break;
                                                }

                                            }
                                        }


                                        else  if ((nextToken == "=" || nextToken == "<") && reached != "yes") {


                                            if (dataGridView1->Rows[w + 3]->Cells[0]->Value == nullptr || dataGridView1->Rows[w + 3]->Cells[0]->Value->ToString() == "") {
                                                DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                                lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                                lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                                dataGridView1->Rows->Add(lastRow); // Add the new row
                                                lastRow->Cells[2]->Value = gcnew String("Wrong until expression");
                                                lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                                                break;
                                            }


                                            DataGridViewRow^ nextRow = dataGridView1->Rows[w + 3];
                                            String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);
                                            if ((!IsIdentifierToken(nextToken) && !IsStringDigit(nextToken))) {
                                                DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                                lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                                lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                                dataGridView1->Rows->Add(lastRow); // Add the new row
                                                lastRow->Cells[2]->Value = gcnew String("Wrong until expression");
                                                lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red;
                                                break;
                                            }


                                            else {


                                                reached = "yes";

                                                continue;







                                            }
                                        }






                                    }
                                }
                            }
                        }



                    if (currentToken == "read" && IsIdentifierToken(nextToken)) {


                        if (dataGridView1->Rows[i + 2]->Cells[0]->Value == nullptr || dataGridView1->Rows[i + 2]->Cells[0]->Value->ToString() == "" || dataGridView1->Rows[i + 2]->Cells[0]->Value->ToString() != ";") {

                            DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                            lastRow->CreateCells(dataGridView1); // Create cells for the new row
                            lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                            dataGridView1->Rows->Add(lastRow); // Add the new row
                            lastRow->Cells[2]->Value = gcnew String("expected semicolumn after statement");
                            lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red; // Highlight s

                            continue;
                        }




                    }

                    if (currentToken == "write" && IsIdentifierToken(nextToken)) {


                        if (dataGridView1->Rows[i + 2]->Cells[0]->Value == nullptr || dataGridView1->Rows[i + 2]->Cells[0]->Value->ToString() == "" || dataGridView1->Rows[i + 2]->Cells[0]->Value->ToString() != ";") {

                            DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                            lastRow->CreateCells(dataGridView1); // Create cells for the new row
                            lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                            dataGridView1->Rows->Add(lastRow); // Add the new row
                            lastRow->Cells[2]->Value = gcnew String("expected semicolumn after statement");
                            lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red; // Highlight s

                            continue;
                        }

                        else if (currentToken == "write" && nextToken == "\"") {
                            for (int w = i; w < dataGridView1->Rows->Count - 1; ++w) {
                                if (dataGridView1->Rows[w + 1]->Cells[0]->Value == nullptr || dataGridView1->Rows[w + 1]->Cells[0]->Value->ToString() == "") {

                                    break;
                                }
                                DataGridViewRow^ nextRow = dataGridView1->Rows[w + 2];
                                String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);
                                if (nextToken != "\"") {
                                    continue;
                                }
                                else if (nextToken == "\"") {
                                    DataGridViewRow^ nextRow = dataGridView1->Rows[w + 2];
                                    String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);
                                    DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                    lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                    lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                    dataGridView1->Rows->Add(lastRow); // Add the new row
                                    lastRow->Cells[2]->Value = gcnew String(nextToken);
                                    lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red; // Highlight s
                                }

                                else if (dataGridView1->Rows[w + 3]->Cells[0]->Value == nullptr || dataGridView1->Rows[w + 3]->Cells[0]->Value->ToString() == "") {
                                    DataGridViewRow^ nextRow = dataGridView1->Rows[w + 2];
                                    String^ nextToken = safe_cast<String^>(nextRow->Cells[0]->Value);
                                    DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                                    lastRow->CreateCells(dataGridView1); // Create cells for the new row
                                    lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                                    dataGridView1->Rows->Add(lastRow); // Add the new row
                                    lastRow->Cells[2]->Value = gcnew String(nextToken);
                                    lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red; // Highlight s
                                }


                            }

                        }




                    }


                    if (IsOperatorToken(currentToken) && (!IsIdentifierToken(nextToken) && !IsStringDigit(nextToken))) {
                        DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                        lastRow->CreateCells(dataGridView1); // Create cells for the new row
                        lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                        dataGridView1->Rows->Add(lastRow); // Add the new row
                        lastRow->Cells[2]->Value = gcnew String("expected identifier or number after operator");
                        lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red; // Highlight s
                    }

                    if (currentToken == "repeat" && (IsOperatorToken(nextToken) || nextToken == ";" || IsStringDigit(nextToken))) {
                        DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                        lastRow->CreateCells(dataGridView1); // Create cells for the new row
                        lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                        dataGridView1->Rows->Add(lastRow); // Add the new row
                        lastRow->Cells[2]->Value = gcnew String("expected identifier or keyword after repeat");
                        lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red; // Highlight s
                    }

                    if (currentToken == "read" && (!IsIdentifierToken(nextToken) )) {
                        DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                        lastRow->CreateCells(dataGridView1); // Create cells for the new row
                        lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                        dataGridView1->Rows->Add(lastRow); // Add the new row
                        lastRow->Cells[2]->Value = gcnew String("Identifier expected after 'read'");
                        lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red; // Highlight s

                    }

                    if (currentToken == "write" &&
                        !(nextToken->StartsWith("\"") && nextToken->EndsWith("\"") && IsIdentifierToken(nextToken->Substring(1, nextToken->Length - 2)))) {

                        DataGridViewRow^ lastRow = gcnew DataGridViewRow(); // Create a new DataGridViewRow
                        lastRow->CreateCells(dataGridView1); // Create cells for the new row
                        lastRow->Cells[3]->Value = currentRow->Cells[3]->Value; // Set the line number
                        dataGridView1->Rows->Add(lastRow); // Add the new row
                        lastRow->Cells[2]->Value = gcnew String("Identifier expected after 'write'");
                        lastRow->Cells[2]->Style->BackColor = System::Drawing::Color::Red; // Highlight the cell
                    }





                }







            }
        }



        bool IsOperatorToken(String^ token) {
            // Add your logic to check if the token is an operator
            return token == "+" || token == "-" || token == "*" || token == "/" || token == "<=" || token == "=" || token == ":=";
        }

        bool IsIdentifierToken(String^ token) {
            // Check if the token is empty or does not start with a letter or underscore
            if (IsKeyword(token) || IsOperatorToken(token) || IsStringDigit(token) || token == ";" || token == "\"") {
                return false;
            }

            // If all characters meet the identifier criteria, it's an identifier
            return true;
        }

        bool IsStringDigit(String^ token) {
            for each (char c in token) {
                if (!Char::IsDigit(c)) {
                    return false;
                }
            }
            return true;
        }

        bool IsKeyword(String^ token) {
            // Add your logic to check if the token is an operator
            return token == "if" || token == "else" || token == "repeat" || token == "until" || token == "read" || token == "write" || token == "then" || token == "end";
        }
    };

}