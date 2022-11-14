#pragma once
#include "FlagController.h"

namespace GE
{
	namespace GameSetting
	{
		class Time
		{
		private:
			static float deltaTime;
			static float applicationElapsedTime;

			static float gameTime;
			static float beforeGameTime;
			static float afterTime;
			static FlagController gameTimeFlagController;
			static bool isLerp;
		public:
			/// <summary>
			/// デルタタイム、アプリ経過時間以外を初期化
			/// </summary>
			static void Initialize();

			/// <summary>
			/// アプリ起動時一度のみ呼ぶ
			/// </summary>
			static void Start();

			/// <summary>
			/// 毎フレーム呼ぶ
			/// </summary>
			/// <param name="frameTime">前フレームの経過時間</param>
			static void Update(float frameTime);

			/// <summary>
			/// 前フレームの経過時間(sec)を取得
			/// </summary>
			/// <returns>前フレームの経過時間(sec)</returns>
			static float GetDeltaTime();

			/// <summary>
			/// アプリを起動してから今までの経過時間(sec)を取得
			/// </summary>
			/// <returns>アプリを起動してから今までの経過時間(sec)</returns>
			static float GetApplicationElapsedTime();

			/// <summary>
			/// ゲーム内時間を取得
			/// </summary>
			/// <returns>ゲーム内時間</returns>
			static float GetGameTime();

			/// <summary>
			/// ゲーム内時間を設定
			/// </summary>
			/// <param name="setTime">設定する時間 ゲームを動かす速度 0 = 動かない 1 = 通常 2 = 倍速 3...</param>
			/// <param name="takeTime">反映させるまでの時間(sec)</param>
			/// <param name="isLerp">イージングで設定時間まで処理させるか</param>
			static void SetGameTime(float setTime, float takeTime = 0,bool switchOptionIsLerp = false);
		};
	}
}
