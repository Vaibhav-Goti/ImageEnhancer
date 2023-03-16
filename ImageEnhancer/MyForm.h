
#include <filesystem>
#include <Exception>
#include <opencv2/opencv.hpp>
#include <msclr/marshal_cppstd.h>
#include <string>



namespace ImageEnhancer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Drawing::Imaging;
	using namespace cv;
	using namespace std;
	using namespace filesystem;


	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
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
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
		FolderBrowserDialog^ dialog = gcnew FolderBrowserDialog;
		FolderBrowserDialog^ savedialog = gcnew FolderBrowserDialog;
	private: System::Windows::Forms::Button^ SelectImageBtn;
	private: System::Windows::Forms::Button^ EnhanceBn;
	private: System::Windows::Forms::ProgressBar^ progressBar;

	protected:

	protected:

	protected:

	protected:


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->SelectImageBtn = (gcnew System::Windows::Forms::Button());
			this->EnhanceBn = (gcnew System::Windows::Forms::Button());
			this->progressBar = (gcnew System::Windows::Forms::ProgressBar());
			this->SuspendLayout();
			// 
			// SelectImageBtn
			// 
			this->SelectImageBtn->Location = System::Drawing::Point(50, 64);
			this->SelectImageBtn->Name = L"SelectImageBtn";
			this->SelectImageBtn->Size = System::Drawing::Size(75, 45);
			this->SelectImageBtn->TabIndex = 0;
			this->SelectImageBtn->Text = L"Select";
			this->SelectImageBtn->UseVisualStyleBackColor = true;
			this->SelectImageBtn->Click += gcnew System::EventHandler(this, &MyForm::SelectImageBtn_Click);
			// 
			// EnhanceBn
			// 
			this->EnhanceBn->Location = System::Drawing::Point(189, 64);
			this->EnhanceBn->Name = L"EnhanceBn";
			this->EnhanceBn->Size = System::Drawing::Size(75, 45);
			this->EnhanceBn->TabIndex = 1;
			this->EnhanceBn->Text = L"Enhance";
			this->EnhanceBn->UseVisualStyleBackColor = true;
			this->EnhanceBn->Click += gcnew System::EventHandler(this, &MyForm::EnhanceBn_Click);
			// 
			// progressBar
			// 
			this->progressBar->Location = System::Drawing::Point(0, 168);
			this->progressBar->Name = L"progressBar";
			this->progressBar->Size = System::Drawing::Size(314, 23);
			this->progressBar->TabIndex = 2;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(312, 193);
			this->Controls->Add(this->progressBar);
			this->Controls->Add(this->EnhanceBn);
			this->Controls->Add(this->SelectImageBtn);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MyForm";
			this->Text = L"ImageEnhancer";
			this->ResumeLayout(false);

		}
#pragma endregion

		// Function to enhance a single image
		void enhanceImage(Mat& inputImage, Mat& outputImage)
		{
			try {
				// Code to enhance image using OpenCV functions goes here
				// For example, sharpening the image:
				Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
				filter2D(inputImage, outputImage, -1, kernel);
				

				//// Apply a bilateral filter to smooth the edges
				//Mat smoothed;
				//bilateralFilter(outputImage, smoothed, 5, 0, 15);

				//// Add the smoothed edges to the input image
				//outputImage = inputImage + smoothed;

				
			}
			catch (System::Exception^ex)
			{
				// handle the exception
				MessageBox::Show("An error occurred: " + ex->Message);
			}
		}
		void removeNoise(Mat& inputImage, Mat& outputImage)
		{
			try {
				// Apply a bilateral filter to smooth the edges
				bilateralFilter(inputImage, outputImage, 9, 175, 175);
			}
			catch (System::Exception^ ex)
			{
				// handle the exception
				MessageBox::Show("An error occurred: " + ex->Message);
			}
		}

	private: System::Void SelectImageBtn_Click(System::Object^ sender, System::EventArgs^ e)
	{
		try
		{
			
			//MessageBox::Show("clicked");
			// Create FolderBrowserDialog to select folder
			dialog->ShowDialog();

			// Store selected folder path
			string folderPath = msclr::interop::marshal_as<string>(dialog->SelectedPath);

		}
		catch (System::Exception^ ex)
		{
			// handle the exception
			MessageBox::Show("An error occurred: " + ex->Message);
		}
	}
	private: System::Void EnhanceBn_Click(System::Object^ sender, System::EventArgs^ e)
	{
		try {
			

			string folderPath = msclr::interop::marshal_as<string>(dialog->SelectedPath);
			savedialog->ShowDialog();
			int numFiles = std::distance(std::filesystem::directory_iterator(folderPath), std::filesystem::directory_iterator());

			progressBar->Minimum = 0;
			progressBar->Maximum = numFiles;
			progressBar->Value = 0;

			int i = 0;
			// Loop through all files in the selected folder
			for (const auto& file : filesystem::directory_iterator(folderPath))
			{
				// Load image using OpenCV
				Mat inputImage = imread(file.path().string());

				// Create output image
				Mat outputImage;
				Mat enhancedImage;

				// Enhance image
				enhanceImage(inputImage, enhancedImage);
				removeNoise(enhancedImage, outputImage);
				
				string savefolderPath = msclr::interop::marshal_as<string>(savedialog->SelectedPath);
				// Save enhanced image to new folder
				string outputPath = savefolderPath + "\\" + file.path().filename().string();
				imwrite(outputPath, outputImage);

				i++;
				progressBar->Value = i;
			}
			MessageBox::Show("images enhanced sucessfully");
			progressBar->Value = 0;
		}
		catch (System::Exception^ ex)
		{
			// handle the exception
			MessageBox::Show("An error occurred: " + ex->Message);
		}

	}


	};
}
