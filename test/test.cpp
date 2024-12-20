#include "board.hpp"
#include "fen.hpp"

#include <gtest/gtest.h>

class ChessTest : public testing::Test {
protected:
	Board board;
};

#define EXPECT_PIECE_TYPE(position, type) EXPECT_EQ((int)board.getPiece(Board::getSquareIdx(position)).getType(), (int)type)
#define MOVE(from, to) Move(board, Board::getSquareIdx(from), Board::getSquareIdx(to))

TEST_F(ChessTest, SquareIdx) {
	EXPECT_FLOAT_EQ(Board::getSquareIdx("a8"), 0u);
	EXPECT_FLOAT_EQ(Board::getSquareIdx("h1"), 63u);
}

TEST_F(ChessTest, ValidMove) {
	FEN::applyFen(board, DEFAULT_FEN);
	EXPECT_TRUE(board.applyMove(MOVE("e2", "e4")));
	EXPECT_PIECE_TYPE("e4", Pawn);
	EXPECT_PIECE_TYPE("e2", None);
}

TEST_F(ChessTest, ValidMove2) {
	FEN::applyFen(board, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
	EXPECT_TRUE(board.applyMove(MOVE("e7", "e5")));
	EXPECT_PIECE_TYPE("e5", Pawn);
	EXPECT_PIECE_TYPE("e7", None);
}

TEST_F(ChessTest, CastlingWhiteQueenSide) {
	FEN::applyFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3KBNR w KQkq - 0 1");
	EXPECT_TRUE(board.applyMove(MOVE("e1", "c1")));
	EXPECT_PIECE_TYPE("c1", King);
	EXPECT_PIECE_TYPE("d1", Rook);
	EXPECT_PIECE_TYPE("e1", None);
	EXPECT_PIECE_TYPE("a1", None);
	EXPECT_PIECE_TYPE("b1", None);
}

TEST_F(ChessTest, CastlingWhiteKingSide) {
	FEN::applyFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1");
	
	EXPECT_TRUE(board.applyMove(MOVE("e1", "g1")));
	EXPECT_PIECE_TYPE("g1", King);
	EXPECT_PIECE_TYPE("f1", Rook);
	EXPECT_PIECE_TYPE("e1", None);
	EXPECT_PIECE_TYPE("h1", None);
}

TEST_F(ChessTest, CastlingBlackQueenSide) {
	FEN::applyFen(board, "r3kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
	
	EXPECT_TRUE(board.applyMove(MOVE("e8", "c8"))); 
	EXPECT_PIECE_TYPE("c8", King);
	EXPECT_PIECE_TYPE("d8", Rook);
	EXPECT_PIECE_TYPE("e8", None);
	EXPECT_PIECE_TYPE("a8", None);
	EXPECT_PIECE_TYPE("b8", None);
}

TEST_F(ChessTest, CastlingBlackKingSide) {
	FEN::applyFen(board, "rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
	
	EXPECT_TRUE(board.applyMove(MOVE("e8", "g8"))); 
	EXPECT_PIECE_TYPE("g8", King);
	EXPECT_PIECE_TYPE("f8", Rook);
	EXPECT_PIECE_TYPE("e8", None);
	EXPECT_PIECE_TYPE("h8", None);
}

TEST_F(ChessTest, CheckMate) {
	FEN::applyFen(board, "6r1/8/8/8/7K/8/8/5r2 b - - 0 1"); 
	EXPECT_TRUE(board.applyMove(MOVE("f1", "h1")));
	EXPECT_TRUE(board.getCheckResult().isCheck);
	EXPECT_EQ(board.getStatus(), BoardStatus::BlackWin); 
}

TEST_F(ChessTest, StaleMate) {
	FEN::applyFen(board, "6r1/8/r7/7K/8/r7/8/6r1 b - - 0 1"); 
	EXPECT_FALSE(board.applyMove(MOVE("a3", "a4")));
	EXPECT_FALSE(board.getCheckResult().isCheck);
	EXPECT_EQ((int)board.getStatus(), (int)BoardStatus::Draw); 
}
