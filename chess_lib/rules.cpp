#include "rules.hpp"
#include "board.hpp"

namespace Rules {
	static std::unordered_map<PieceType, AddLegalMovesFunc> s_getLegalMovesFuncMap {
		ADD_LEGAL_MOVES_MAP_ENTRY(Pawn),   ADD_LEGAL_MOVES_MAP_ENTRY(Rook), ADD_LEGAL_MOVES_MAP_ENTRY(Bishop),
		ADD_LEGAL_MOVES_MAP_ENTRY(Knight), ADD_LEGAL_MOVES_MAP_ENTRY(Queen), ADD_LEGAL_MOVES_MAP_ENTRY(King),
	};

	bool isMoveLegal(const std::vector<Move> &legalMoves, const Move &move) {
		for(const Move &legalMove : legalMoves)
			if(legalMove == move)
				return true;

		return false;
	}

	void addLegalMoves(std::vector<Move> &legalMoves, Board &board, const uint8_t idx, const bool ignoreCheck) {
		const Piece &piece = board.getPiece(idx);

		const std::uint8_t pieceX = idx % 8;
		const std::uint8_t pieceY = idx / 8;
		const AddLegalMoveContext ctx = {.legalMoves = legalMoves, .board = board, .piece = piece, .pieceIdx = idx, .pieceX = pieceX, .pieceY = pieceY, .ignoreCheck = ignoreCheck};

		s_getLegalMovesFuncMap[piece.getType()](ctx);
	}

	bool isInCheckAfterMove(Board &board, const Move &move) {
		board.applyMove(move, true, true);
		bool isCheck = board.getCheckResult().isCheck;
		board.revertLastMove();
		return isCheck;
	}

	void addLegalMove(const AddLegalMoveContext &ctx, const std::uint8_t targetIdx) {
		const Move move = Move(ctx.board, ctx.pieceIdx, targetIdx);

		if(!ctx.ignoreCheck && isInCheckAfterMove(ctx.board, move))
			return;

		ctx.legalMoves.push_back(move);
	}

	bool addMoveIfEmpty(const AddLegalMoveContext &ctx, const std::uint8_t targetIdx) {
		if(!Board::isSquareIdxCorrect(targetIdx) || !ctx.board.getPiece(targetIdx).isNull())
			return false;

		addLegalMove(ctx, targetIdx);
		return true;
	}

	bool addMoveIfHasOpponentPiece(const AddLegalMoveContext &ctx, const std::uint8_t targetIdx) {
		const Piece &targetPiece = ctx.board.getPiece(targetIdx);
		if(!Board::isSquareIdxCorrect(targetIdx) || targetPiece.isNull() || targetPiece.isColor(ctx.piece.getColor()))
			return false;

		addLegalMove(ctx, targetIdx);
		return true;
	}

	bool addMoveIfNotBlocked(const AddLegalMoveContext &ctx, const std::uint8_t targetIdx) {
		if(!Board::isSquareIdxCorrect(targetIdx))
			return false;

		const Piece &targetPiece = ctx.board.getPiece(targetIdx);
		if(!targetPiece.isNull() && ctx.piece.isColor(targetPiece.getColor()))
			return false;

		addLegalMove(ctx, targetIdx);
		return true;
	}
}
