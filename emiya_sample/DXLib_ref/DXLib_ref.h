#pragma once

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*ビルド設定																																*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
#define DEBUG (TRUE)/*デバッグ表示を入れるか否か*/

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*pragma																																	*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma warning(disable:4505)	/*使用されていない関数を破棄する旨の警告を取るために追加*/

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*include																																	*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
// 共通
#include <array>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <algorithm>
#include <D3D11.h>
#include <cmath>

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*const																																		*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
constexpr float		FrameRate{ 60.f };								/*処理の基準になるフレームレート*/
constexpr float		GravityRate{ -9.8f };							/*重力加速度*/
constexpr int		InvalidID{ -1 };								/*共通の無効値*/
constexpr int		BaseScreenWidth{ 1920 };						/*UI描画などの基準となる解像度*/
constexpr int		BaseScreenHeight{ 1080 };						/*UI描画などの基準となる解像度*/

// DPIを反映するデスクトップサイズ
const int deskx{ static_cast<int>(GetSystemMetrics(SM_CXSCREEN)) };
const int desky{ static_cast<int>(GetSystemMetrics(SM_CYSCREEN)) };

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*include DXLIB																																*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
// DXLIB
#include "DxLib.h"
// 追加
#include "DXLib_ref_00_Util.hpp"
#include "DXLib_ref_01_Algorithm.hpp"
#include "DXLib_ref_02_Sound.hpp"
#include "DXLib_ref_03_GraphHandle.hpp"
#include "DXLib_ref_08_Etc.hpp"
#include "DXLib_ref_09_KeyControl.hpp"
#include "DXLib_ref_10_Option.hpp"
#include "DXLib_ref_11_FileControl.hpp"
#include "DXLib_ref_12_Draw2DSystem.hpp"
#include "DXLib_ref_20_Debug.hpp"
#include "DXLib_ref_40_DXDraw.hpp"
#include "DXLib_ref_100_PostPass.hpp"
#include "DXLib_ref_101_sequence.hpp"
#include "DXLib_ref_102_ObjectManager.hpp"

namespace DXLibRef {
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	/*DXLibRefのメインループなど全般的な管理を行います																							*/
	/*------------------------------------------------------------------------------------------------------------------------------------------*/
	class DXLib_ref : public SingletonBase<DXLib_ref> {
	private:
		friend class SingletonBase<DXLib_ref>;
	private:
		// コンストラクタ
		DXLib_ref(void) noexcept;// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
		DXLib_ref(const DXLib_ref&) = delete;
		DXLib_ref(DXLib_ref&& o) = delete;
		DXLib_ref& operator=(const DXLib_ref&) = delete;
		DXLib_ref& operator=(DXLib_ref&& o) = delete;
		// デストラクタはシングルトンなので呼ばれません
	public:
		// メインループやシーン遷移管理を行う
		bool			MainLogic(void) noexcept;
	};
};
