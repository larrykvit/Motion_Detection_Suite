/*
K&M softaware
Written by Jack Moseley and Larry Kvitnevski

The code for the gui.
Code written by K&M starts at line 420, the rest was automatically generated
*/
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

#include <msclr\marshal_cppstd.h>

using namespace std;

//struct that contains all the parameters
struct Parameter
{
	string filePathOpen, filePathSave, imgName;
	int imgStartNum, imgEndNum, colourThreshold, searchRadius, minTargetSize, numOfTargets;
	double sizeThreshold;
};

//opens file and fills the struct
void fileOpen(Parameter* para);

//convert to std::string from system string
string convertToString( System::String^ input );

//converts to system::string from std::string
System::String^ convertToUString( string input );

#pragma once

namespace File_create {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 
	private: System::Windows::Forms::Button^  btn_load;

	private: System::Windows::Forms::Button^  btn_save;
	private: System::Windows::Forms::TextBox^  txt_fileOpen;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  txt_fileSave;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  txt_imgName;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::NumericUpDown^  num_startNum;
	private: System::Windows::Forms::NumericUpDown^  num_endNum;


	private: System::Windows::Forms::NumericUpDown^  num_searchRad;
	private: System::Windows::Forms::NumericUpDown^  num_clrTh;




	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::NumericUpDown^  num_minSize;

	private: System::Windows::Forms::NumericUpDown^  num_sizeTh;

	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::NumericUpDown^  num_numTargets;

	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label12;

	private: System::Windows::Forms::Button^  btn_run;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btn_load = (gcnew System::Windows::Forms::Button());
			this->btn_save = (gcnew System::Windows::Forms::Button());
			this->txt_fileOpen = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->txt_fileSave = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->txt_imgName = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->num_startNum = (gcnew System::Windows::Forms::NumericUpDown());
			this->num_endNum = (gcnew System::Windows::Forms::NumericUpDown());
			this->num_searchRad = (gcnew System::Windows::Forms::NumericUpDown());
			this->num_clrTh = (gcnew System::Windows::Forms::NumericUpDown());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->num_minSize = (gcnew System::Windows::Forms::NumericUpDown());
			this->num_sizeTh = (gcnew System::Windows::Forms::NumericUpDown());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->num_numTargets = (gcnew System::Windows::Forms::NumericUpDown());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->btn_run = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num_startNum))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num_endNum))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num_searchRad))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num_clrTh))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num_minSize))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num_sizeTh))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num_numTargets))->BeginInit();
			this->SuspendLayout();
			// 
			// btn_load
			// 
			this->btn_load->Location = System::Drawing::Point(162, 331);
			this->btn_load->Name = L"btn_load";
			this->btn_load->Size = System::Drawing::Size(124, 38);
			this->btn_load->TabIndex = 1;
			this->btn_load->Text = L"Load";
			this->btn_load->UseVisualStyleBackColor = true;
			this->btn_load->Click += gcnew System::EventHandler(this, &Form1::btn_load_Click);
			// 
			// btn_save
			// 
			this->btn_save->Location = System::Drawing::Point(162, 280);
			this->btn_save->Name = L"btn_save";
			this->btn_save->Size = System::Drawing::Size(124, 38);
			this->btn_save->TabIndex = 3;
			this->btn_save->Text = L"Save";
			this->btn_save->UseVisualStyleBackColor = true;
			this->btn_save->Click += gcnew System::EventHandler(this, &Form1::btn_save_Click);
			// 
			// txt_fileOpen
			// 
			this->txt_fileOpen->Location = System::Drawing::Point(62, 78);
			this->txt_fileOpen->Name = L"txt_fileOpen";
			this->txt_fileOpen->Size = System::Drawing::Size(178, 20);
			this->txt_fileOpen->TabIndex = 4;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(59, 59);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 13);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Read Location";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(59, 101);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(76, 13);
			this->label2->TabIndex = 6;
			this->label2->Text = L"Save Location";
			// 
			// txt_fileSave
			// 
			this->txt_fileSave->Location = System::Drawing::Point(62, 117);
			this->txt_fileSave->Name = L"txt_fileSave";
			this->txt_fileSave->Size = System::Drawing::Size(178, 20);
			this->txt_fileSave->TabIndex = 7;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(59, 179);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(61, 13);
			this->label3->TabIndex = 10;
			this->label3->Text = L"Start Image";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(59, 140);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(67, 13);
			this->label4->TabIndex = 9;
			this->label4->Text = L"Image Name";
			// 
			// txt_imgName
			// 
			this->txt_imgName->Location = System::Drawing::Point(62, 156);
			this->txt_imgName->Name = L"txt_imgName";
			this->txt_imgName->Size = System::Drawing::Size(178, 20);
			this->txt_imgName->TabIndex = 8;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(59, 218);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(58, 13);
			this->label5->TabIndex = 12;
			this->label5->Text = L"End Image";
			// 
			// num_startNum
			// 
			this->num_startNum->Location = System::Drawing::Point(62, 195);
			this->num_startNum->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->num_startNum->Name = L"num_startNum";
			this->num_startNum->Size = System::Drawing::Size(178, 20);
			this->num_startNum->TabIndex = 13;
			// 
			// num_endNum
			// 
			this->num_endNum->Location = System::Drawing::Point(62, 234);
			this->num_endNum->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->num_endNum->Name = L"num_endNum";
			this->num_endNum->Size = System::Drawing::Size(178, 20);
			this->num_endNum->TabIndex = 14;
			// 
			// num_searchRad
			// 
			this->num_searchRad->Location = System::Drawing::Point(309, 117);
			this->num_searchRad->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {32000, 0, 0, 0});
			this->num_searchRad->Name = L"num_searchRad";
			this->num_searchRad->Size = System::Drawing::Size(178, 20);
			this->num_searchRad->TabIndex = 18;
			// 
			// num_clrTh
			// 
			this->num_clrTh->Location = System::Drawing::Point(309, 78);
			this->num_clrTh->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
			this->num_clrTh->Name = L"num_clrTh";
			this->num_clrTh->Size = System::Drawing::Size(178, 20);
			this->num_clrTh->TabIndex = 17;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(306, 101);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(77, 13);
			this->label6->TabIndex = 16;
			this->label6->Text = L"Search Radius";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(306, 62);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(87, 13);
			this->label7->TabIndex = 15;
			this->label7->Text = L"Colour Threshold";
			// 
			// num_minSize
			// 
			this->num_minSize->Location = System::Drawing::Point(309, 196);
			this->num_minSize->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {32000, 0, 0, 0});
			this->num_minSize->Name = L"num_minSize";
			this->num_minSize->Size = System::Drawing::Size(178, 20);
			this->num_minSize->TabIndex = 22;
			// 
			// num_sizeTh
			// 
			this->num_sizeTh->DecimalPlaces = 2;
			this->num_sizeTh->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
			this->num_sizeTh->Location = System::Drawing::Point(309, 157);
			this->num_sizeTh->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->num_sizeTh->Name = L"num_sizeTh";
			this->num_sizeTh->Size = System::Drawing::Size(178, 20);
			this->num_sizeTh->TabIndex = 21;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(306, 180);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(105, 13);
			this->label8->TabIndex = 20;
			this->label8->Text = L"Minimum Target Size";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(306, 141);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(77, 13);
			this->label9->TabIndex = 19;
			this->label9->Text = L"Size Threshold";
			// 
			// num_numTargets
			// 
			this->num_numTargets->Location = System::Drawing::Point(309, 234);
			this->num_numTargets->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {32000, 0, 0, 0});
			this->num_numTargets->Name = L"num_numTargets";
			this->num_numTargets->Size = System::Drawing::Size(178, 20);
			this->num_numTargets->TabIndex = 24;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(306, 218);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(95, 13);
			this->label10->TabIndex = 23;
			this->label10->Text = L"Number of Targets";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label11->Location = System::Drawing::Point(115, 19);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(51, 24);
			this->label11->TabIndex = 25;
			this->label11->Text = L"Input";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label12->Location = System::Drawing::Point(343, 19);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(112, 24);
			this->label12->TabIndex = 26;
			this->label12->Text = L"Preferences";
			// 
			// btn_run
			// 
			this->btn_run->Location = System::Drawing::Point(342, 280);
			this->btn_run->Name = L"btn_run";
			this->btn_run->Size = System::Drawing::Size(124, 89);
			this->btn_run->TabIndex = 28;
			this->btn_run->Text = L"Run";
			this->btn_run->UseVisualStyleBackColor = true;
			this->btn_run->Click += gcnew System::EventHandler(this, &Form1::btn_run_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(537, 408);
			this->Controls->Add(this->btn_run);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->num_numTargets);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->num_minSize);
			this->Controls->Add(this->num_sizeTh);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->num_searchRad);
			this->Controls->Add(this->num_clrTh);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->num_endNum);
			this->Controls->Add(this->num_startNum);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->txt_imgName);
			this->Controls->Add(this->txt_fileSave);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->txt_fileOpen);
			this->Controls->Add(this->btn_save);
			this->Controls->Add(this->btn_load);
			this->Name = L"Form1";
			this->Text = L"File create";
			this->Load += gcnew System::EventHandler(this, &Form1::btn_load_Click);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num_startNum))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num_endNum))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num_searchRad))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num_clrTh))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num_minSize))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num_sizeTh))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->num_numTargets))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btn_load_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			Parameter para;
			fileOpen( &para );
			
			Decimal sizeThreshold( para.sizeThreshold );

			txt_fileOpen->Text = convertToUString(para.filePathOpen);
			txt_fileSave->Text = convertToUString(para.filePathSave);
			txt_imgName->Text = convertToUString(para.imgName);
			num_startNum->Value = para.imgStartNum;
			num_endNum->Value = para.imgEndNum;
			num_clrTh->Value = para.colourThreshold;
			num_searchRad->Value = para.searchRadius;
			num_sizeTh->Value = sizeThreshold;
			num_minSize->Value = para.minTargetSize;
			num_numTargets->Value = para.numOfTargets;
		}
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) 
		{
			
		}
	private: System::Void btn_save_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			//takes the value from gui
			string fileOpen = convertToString(txt_fileOpen->Text);
			string fileSave = convertToString(txt_fileSave->Text);
			string imgName = convertToString(txt_imgName->Text);
			int startNum = (int)num_startNum->Value;
			int endNum = (int)num_endNum->Value;
			int clrTh = (int)num_clrTh->Value;
			int searchRad = (int)num_searchRad->Value;
			double sizeTh = (double)num_sizeTh->Value;
			int minSize = (int)num_minSize->Value;
			int numTargets = (int)num_numTargets->Value;

			//saves them to the files
			ofstream fileInS("input.txt");
			fileInS << fileOpen << endl << fileSave << endl << imgName << endl << startNum << endl << endNum;
			fileInS.close();
			ofstream filePrefS("preferences.txt");
			filePrefS << clrTh << endl << searchRad << endl << sizeTh << endl << minSize << endl << numTargets;
			filePrefS.close();
		}
private: System::Void btn_run_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			//takes values from gui
			string fileOpen = convertToString(txt_fileOpen->Text);
			string fileSave = convertToString(txt_fileSave->Text);
			string imgName = convertToString(txt_imgName->Text);
			int startNum = (int)num_startNum->Value;
			int endNum = (int)num_endNum->Value;
			int clrTh = (int)num_clrTh->Value;
			int searchRad = (int)num_searchRad->Value;
			double sizeTh = (double)num_sizeTh->Value;
			int minSize = (int)num_minSize->Value;
			int numTargets = (int)num_numTargets->Value;

			//saves them
			ofstream fileInS("input.txt");
			fileInS << fileOpen << endl << fileSave << endl << imgName << endl << startNum << endl << endNum;
			fileInS.close();
			ofstream filePrefS("preferences.txt");
			filePrefS << clrTh << endl << searchRad << endl << sizeTh << endl << minSize << endl << numTargets;
			filePrefS.close();
			 
			 //runs the motion detection program
			 system( "\"Motion detection.exe\"" );
		 }
};
}

//converts from unmanaged string to std::string
string convertToString( System::String^ input )
{
	System::String^ s = input;

	msclr::interop::marshal_context context;
	string output = context.marshal_as<std::string>(s);
	return output;
}

//converts from string to unmanaged string
System::String^ convertToUString( string input )
{
	System::String^ output = gcnew System::String(input.c_str());
	return output;
}

void fileOpen(Parameter* para)
{
	// Numeric values are later converted to reduce errors from incorrect characters in fields
	string filePathOpen, filePathSave, imgName, imgStartNum, imgEndNum, colourThreshold, searchRadius, sizeThreshold, minTargetSize, numOfTargets;
	// Open the input file
	ifstream inputFile ("input.txt");
	// Check if the file has been successfully opened
	if (inputFile.is_open())
	{
		// Take and save information from the file line by line
		// getline used as file paths may have spaces
		getline(inputFile, filePathOpen);
		getline(inputFile, filePathSave);
		inputFile >> imgName >> imgStartNum >> imgEndNum;
		inputFile.close();
	}
	
	// Read the preferences file
	ifstream prefFileIn ("preferences.txt");
	if (prefFileIn.is_open())
	{
		prefFileIn >> colourThreshold >> searchRadius >> sizeThreshold >> minTargetSize >> numOfTargets;
		prefFileIn.close();
	}
	// If the preferences file is not found create one with default values
	else
	{
		colourThreshold = "20"; 
		searchRadius = "6000";
		sizeThreshold = "0.5";
		minTargetSize = "100";
		numOfTargets = "10";

		ofstream prefFileOut ("preferences.txt");
		prefFileOut << colourThreshold << endl << searchRadius << endl << sizeThreshold << endl << minTargetSize << endl << numOfTargets;
		prefFileOut.close();
	}
	// Store all settings in structure para
	para->filePathOpen = filePathOpen;
	para->filePathSave = filePathSave;
	para->imgName = imgName;
	para->imgStartNum = atoi( imgStartNum.c_str() );
	para->imgEndNum = atoi( imgEndNum.c_str() );
	para->colourThreshold = atoi(colourThreshold.c_str());
	para->searchRadius = atoi(searchRadius.c_str());
	para->sizeThreshold = atof(sizeThreshold.c_str());
	para->minTargetSize = atoi(minTargetSize.c_str());
	para->numOfTargets = atoi(numOfTargets.c_str());
}