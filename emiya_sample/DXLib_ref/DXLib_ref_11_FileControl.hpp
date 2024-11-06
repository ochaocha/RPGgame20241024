#pragma once
#include "DXLib_ref.h"

#include <fstream>
#include <filesystem>

namespace DXLibRef {
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	/*ファイル操作系関数																														*/
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// クリップボードに画像をコピー
	static auto GetClipBoardGraphHandle(GraphHandle* pAnswerHandle) noexcept {
		HWND  hwnd = GetMainWindowHandle();

		if (IsClipboardFormatAvailable(CF_BITMAP) == FALSE) {
			return false;
		}	// 指定した形式のデータがクリップボードにあるかを問い合わせる
		if (OpenClipboard(hwnd) == FALSE) {
			return false;
		}						// オープン
		HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);					// データ取得
		CloseClipboard();														// クローズ
		if (hBitmap == NULL) {
			return false;
		}

		BITMAP bitmap{};
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);

		BITMAPINFO bmpInfo{};
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfo.bmiHeader.biWidth = bitmap.bmWidth;
		bmpInfo.bmiHeader.biHeight = bitmap.bmHeight;
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = 32;
		bmpInfo.bmiHeader.biCompression = BI_RGB;

		size_t Siz = static_cast<size_t>(bitmap.bmWidth * bitmap.bmHeight * (LONG)4);
		BYTE* bmpData = new BYTE[Siz];
		HDC hDC = GetDC(hwnd);
		GetDIBits(hDC, hBitmap, 0, static_cast<UINT>(bitmap.bmHeight), (void*)bmpData, &bmpInfo, DIB_RGB_COLORS);
		ReleaseDC(hwnd, hDC);
		DeleteObject(hBitmap);

		pAnswerHandle->CreateGraphFromBmp(&bmpInfo, bmpData);	// ハンドルに変換
		delete[] bmpData;
		return true;
	}
	// ファイルが存在するかチェック
	static bool IsFileExist(const char* Path) noexcept {
		return std::filesystem::is_regular_file(Path);
		//FILEINFO FileInfo;
		//return (FileRead_findFirst(Path, &FileInfo) != (DWORD_PTR)InvalidID);
	}
	// ディレクトリ内のファイル走査
	static void GetFileNamesInDirectory(const char* pPath, std::vector<WIN32_FIND_DATA>* pData) noexcept {
		pData->clear();
		WIN32_FIND_DATA win32fdt;
		HANDLE hFind = FindFirstFile(pPath, &win32fdt);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (win32fdt.cFileName[0] != '.') {
					pData->resize(pData->size() + 1);
					pData->back() = win32fdt;
				}

			} while (FindNextFile(hFind, &win32fdt));
		} // else{ return false; }
		FindClose(hFind);
	}
	// 特定のパスの実行ファイルを起動
	static void CreateOurProcess(char* szCmd, DWORD flag, bool fWait) noexcept {
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		memset(&si, 0, sizeof(STARTUPINFO));
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));
		si.cb = sizeof(STARTUPINFO);
		CreateProcess(NULL, szCmd, NULL, NULL, FALSE, flag, NULL, NULL, &si, &pi);
		if (fWait) WaitForSingleObject(pi.hProcess, INFINITE);	// 終了を待つ.
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	// 自身を別途起動(再起動などの際に利用します)
	static void StartMe(void) noexcept {
		char Path[MAX_PATH];
		// EXEのあるフォルダのパスを取得
		::GetModuleFileName(NULL, Path, MAX_PATH);
		CreateOurProcess(Path, SW_HIDE, false);
	}

	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// ファイル選択ダイアログを出せるクラス
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	class DialogManager {
		OPENFILENAME	ofn;
		TCHAR			strFile[MAX_PATH]{ 0 };
		TCHAR			cdir[MAX_PATH]{ 0 };
	public:
		DialogManager(void) noexcept {
			char Path[MAX_PATH];
			GetModuleFileName(NULL, Path, MAX_PATH);			// EXEのあるフォルダのパスを取得
			SetCurrentDirectory(Path);							// カレントディレクトリの設定
		}
		~DialogManager(void) noexcept {}
	public:
		void			Initialize(void) noexcept {
			GetCurrentDirectory(MAX_PATH, cdir);
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = GetMainWindowHandle();
			ofn.lpstrFilter =
				TEXT("Picture files {*.bmp}\0*.bmp\0")
				TEXT("Picture files {*.dds}\0*.dds\0")
				TEXT("Picture files {*.jpg}\0*.jpg\0")
				TEXT("Picture files {*.png}\0*.png\0");
			ofn.lpstrCustomFilter = NULL;
			ofn.nMaxCustFilter = NULL;
			ofn.nFilterIndex = 0;
			ofn.lpstrFile = strFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
			ofn.lpstrInitialDir = cdir;
			ofn.lpstrTitle = "";
		}
		bool			Open(void) noexcept {
			ofn.lpstrTitle = "開く";
			return GetOpenFileName(&ofn);
		}
		bool			Save(void) noexcept {
			ofn.lpstrTitle = "保存";
			return GetSaveFileName(&ofn);
		}
		const auto* GetPath(void) noexcept {
			std::string str = strFile;
			return (str.find(cdir) != std::string::npos) ? &strFile[strlen(cdir) + 1] : strFile;
		}
	};

	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	// 汎用セーブデータクラス
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	typedef std::pair<std::string, int64_t> SaveParam;
	class SaveDataClass : public SingletonBase<SaveDataClass> {
	private:
		friend class SingletonBase<SaveDataClass>;
	private:
		std::vector<SaveParam> m_data;
	private:
		// コンストラクタ
		SaveDataClass(void) noexcept {// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
			Load();
		}
		SaveDataClass(const SaveDataClass&) = delete;
		SaveDataClass(SaveDataClass&& o) = delete;
		SaveDataClass& operator=(const SaveDataClass&) = delete;
		SaveDataClass& operator=(SaveDataClass&& o) = delete;
		// デストラクタはシングルトンなので呼ばれません
	public:
		SaveParam* GetData(const std::string& Name) noexcept {
			for (auto& d : m_data) {
				if (d.first == Name) {
					return &d;
				}
			}
			return nullptr;
		}
	public:
		void SetParam(const std::string& Name, int64_t value) noexcept {
			auto* Data = GetData(Name);
			if (Data) {
				Data->second = value;
			}
			else {
				m_data.emplace_back(std::make_pair((std::string)Name, value));
			}
		}
		auto GetParam(const std::string& Name) noexcept {
			auto* Data = GetData(Name);
			if (Data) {
				return Data->second;
			}
			return (int64_t)InvalidID;
		}
	public:
		void Save(void) noexcept {
			std::ofstream outputfile("Save/new.svf");
			for (auto& d : m_data) {
				outputfile << d.first + "=" + std::to_string(d.second) + "\n";
			}
			outputfile.close();
		}
		bool Load(void) noexcept {

			m_data.clear();

			std::ifstream inputputfile("Save/new.svf");
			if (!inputputfile) {
				return false;
			}
			std::string line;
			while (std::getline(inputputfile, line)) {
				auto Start = line.find("=");
				if (Start != std::string::npos) {
					m_data.emplace_back(std::make_pair(line.substr(0, Start), std::stoi(line.substr(Start + 1))));
				}
			}
			inputputfile.close();
			return true;
		}
		void Dispose(void) noexcept {
			m_data.clear();
		}
	};
};
