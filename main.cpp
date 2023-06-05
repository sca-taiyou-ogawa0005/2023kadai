#include <DxLib.h>
#include <vector>
#include <random>
using namespace std;

// ウィンドウサイズ
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

// パズルのサイズ
const int PUZZLE_SIZE = 4;
const int PUZZLE_PIECE_SIZE = 80;

// パズルピースの色
const int PIECE_COLOR = GetColor(255, 255, 255);

// パズルの状態を表す2次元配列
std::vector<std::vector<int>> puzzleBoard(PUZZLE_SIZE, std::vector<int>(PUZZLE_SIZE));

// 空きスペースの位置
int emptyRow = PUZZLE_SIZE - 1;
int emptyCol = PUZZLE_SIZE - 1;

// ランダムな数値を生成するための乱数エンジン
std::random_device rd;
std::mt19937 rng(rd());

// ゲームの初期化
void InitializeGame()
{
    // パズルの初期配置を生成
    int number = 1;
    for (int row = 0; row < PUZZLE_SIZE; ++row)
    {
        for (int col = 0; col < PUZZLE_SIZE; ++col)
        {
            puzzleBoard[row][col] = number;
            ++number;
        }
    }
    puzzleBoard[emptyRow][emptyCol] = 0;

    // パズルをシャッフルする
    std::shuffle(puzzleBoard.begin(), puzzleBoard.end(), rng);
}

// パズルを描画する
void DrawPuzzle()
{
    for (int row = 0; row < PUZZLE_SIZE; ++row)
    {
        for (int col = 0; col < PUZZLE_SIZE; ++col)
        {
            int number = puzzleBoard[row][col];
            if (number != 0)
            {
                int x = col * PUZZLE_PIECE_SIZE;
                int y = row * PUZZLE_PIECE_SIZE;
                DrawBox(x, y, x + PUZZLE_PIECE_SIZE, y + PUZZLE_PIECE_SIZE, PIECE_COLOR, TRUE);
                DrawFormatString(x + 10, y + 10, GetColor(0, 0, 0), "%d", number);
            }
        }
    }
}

// パズルピースの移動
void MovePiece(int row, int col)
{
    if ((row == emptyRow && (col == emptyCol - 1 || col == emptyCol + 1)) ||
        (col == emptyCol && (row == emptyRow - 1 || row == emptyRow + 1)))
    {
        puzzleBoard[emptyRow][emptyCol] = puzzleBoard[row][col];
        puzzleBoard[row][col] = 0;
        emptyRow = row;
        emptyCol = col;
    }
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // ウィンドウを作成
    ChangeWindowMode(TRUE);
    SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);
    SetMainWindowText("15 Puzzle");
    if (DxLib_Init() == -1)
    {
        return -1;
    }

    // ゲームの初期化
    InitializeGame();

    // タイマーの開始
    int startTime = GetNowCount();

    while (ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE))
    {
        if (ClearDrawScreen() == 0)
        {
            // パズルを描画
            DrawPuzzle();

            // パズルピースの移動
            int mouseX, mouseY;
            GetMousePoint(&mouseX, &mouseY);

            int row = mouseY / PUZZLE_PIECE_SIZE;
            int col = mouseX / PUZZLE_PIECE_SIZE;

            if (GetMouseInput() & MOUSE_INPUT_LEFT)
            {
                MovePiece(row, col);
            }

            // パズルがクリアされたかチェック
            bool isClear = true;
            int number = 1;
            for (int row = 0; row < PUZZLE_SIZE; ++row)
            {
                for (int col = 0; col < PUZZLE_SIZE; ++col)
                {
                    if (puzzleBoard[row][col] != number)
                    {
                        isClear = false;
                        break;
                    }
                    ++number;
                }
                if (!isClear)
                {
                    break;
                }
            }

            // クリアした場合、タイマーを停止してクリアタイムを表示
            if (isClear)
            {
                int endTime = GetNowCount();
                int clearTime = endTime - startTime;
                DrawFormatString(10, WINDOW_HEIGHT - 30, GetColor(255, 255, 255), "Clear Time: %d ms", clearTime);
            }

            ScreenFlip();
        }
    }

    DxLib_End();
    return 0;
}
