#pragma once
/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*DXLIB�ɒ��ڂ������̂Ȃ����m  																											*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

namespace DXLibRef {
	// --------------------------------------------------------------------------------------------------
	// �V���O���g�� �C���^�[�t�F�[�X�ɉ������p�^�[��������������N���X
	// --------------------------------------------------------------------------------------------------
	template <class T>
	class SingletonBase {
	private:
		static const T* m_Singleton;
	public:
		static void Create(void) noexcept {
			m_Singleton = new T();
		}
		static T* Instance(void) noexcept {
			if (m_Singleton == nullptr) {
				MessageBox(NULL, "Failed Instance Create", "", MB_OK);
				exit(InvalidID);
			}
			// if (m_Singleton == nullptr) { m_Singleton = new T(); }
			return (T*)m_Singleton;
		}
	protected:
		SingletonBase(void) noexcept {}
		// (�|�C���^�œ��I�m�ۂ���X�^�C���̃V���O���g���Ȃ̂ŌĂ΂�܂���)
		// virtual ~SingletonBase(void) noexcept {}
	private:
		SingletonBase(const SingletonBase&) = delete;// �R�s�[���Ă͂����Ȃ��̂ł��ׂ�delete
		SingletonBase& operator=(const SingletonBase&) = delete;
		SingletonBase(SingletonBase&&) = delete;
		SingletonBase& operator=(SingletonBase&&) = delete;
	};
	// �q�̃T���v��
	/*
	class A : public SingletonBase<A> {
	private:
		friend class SingletonBase<A>;
	}
	// �V���O���g���̎��Ԓ�`(cpp�ɋL�q)
	const A* SingletonBase<A>::m_Singleton = nullptr;
	*/

	// --------------------------------------------------------------------------------------------------
	// DXLIB�̃n���h������n�̊��N���X
	// --------------------------------------------------------------------------------------------------
	class DXHandle {
	private:
		int m_handle{ InvalidID };
	protected:
		constexpr DXHandle(int h) noexcept : m_handle(h) {}
	public:
		constexpr DXHandle(void) noexcept : m_handle(InvalidID) {}
		DXHandle(const DXHandle&) = delete;// �R�s�[���Ă͂����Ȃ��̂ł��ׂ�delete
		DXHandle(DXHandle&& o) noexcept : m_handle(o.get()) { o.SetHandleDirect(InvalidID); }
		DXHandle& operator=(const DXHandle&) = delete;
		DXHandle& operator=(DXHandle&& o) noexcept {
			SetHandleDirect(o.get());
			o.SetHandleDirect(InvalidID);
			return *this;
		}

		virtual ~DXHandle(void) noexcept {
			Dispose();
		}
	public:
		const int get(void) const noexcept { return this->m_handle; }
		constexpr bool IsActive(void) const noexcept { return this->m_handle != InvalidID; }
		constexpr explicit operator bool(void) const noexcept { return IsActive(); }
	public:
		void Dispose(void) noexcept {
			if (IsActive()) {
				Dispose_Sub();
				SetHandleDirect(InvalidID);
			}
		}
	protected:
		void SetHandleDirect(int handle) noexcept { this->m_handle = handle; }
		virtual void Dispose_Sub(void) noexcept {}
	};

	// --------------------------------------------------------------------------------------------------
	// �����ϊ�
	// --------------------------------------------------------------------------------------------------
	/*wstring��string�֕ϊ�*/
	static std::string WStringToString(const std::wstring& oWString) noexcept {
		// wstring �� SJIS
		int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.data(), int(oWString.size() + 1), nullptr, 0, NULL, NULL);
		// �o�b�t�@�̎擾
		CHAR* cpMultiByte = new CHAR[static_cast<size_t>(iBufferSize)];
		// wstring �� SJIS
		WideCharToMultiByte(CP_OEMCP, 0, oWString.data(), int(oWString.size() + 1), cpMultiByte, iBufferSize, NULL, NULL);
		// string�̐���
		std::string oAnswer(cpMultiByte, cpMultiByte + iBufferSize - 1);
		// �o�b�t�@�̔j��
		delete[] cpMultiByte;
		// �ϊ����ʂ�Ԃ�
		return oAnswer;
	}
	/*string��wstring�֕ϊ�����*/
	static std::wstring StringToWString(const std::string& oString) noexcept {
		// SJIS �� wstring
		int iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.data(), int(oString.size() + 1), nullptr, 0);
		// �o�b�t�@�̎擾
		wchar_t* cpUCS2 = new wchar_t[static_cast<size_t>(iBufferSize)];
		// SJIS �� wstring
		MultiByteToWideChar(CP_ACP, 0, oString.data(), int(oString.size() + 1), cpUCS2, iBufferSize);
		// string�̐���
		std::wstring oAnswer(cpUCS2, cpUCS2 + iBufferSize - 1);
		// �o�b�t�@�̔j��
		delete[] cpUCS2;
		// �ϊ����ʂ�Ԃ�
		return oAnswer;
	}
}
