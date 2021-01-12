#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <set>
#include <Windows.h>

//인덱스 구조체
struct TIndex
{
	int x, y;
	bool operator == (const TIndex& pos)
	{
		return (x == pos.x && y == pos.y);
	}
	TIndex operator + (const TIndex& right_)
	{
		return{ x + right_.x, y + right_.y };
	}
	TIndex(int fX, int fY)
	{
		x = fX;
		y = fY;
	}
	TIndex()
	{
		x = y = 0;
	}
};

namespace AStar
{
	using uint = unsigned int;
	//함수포인터, 거리계산 함수 설정
	using HeuristicFunction = std::function<uint(TIndex, TIndex)>;
	//좌표 리스트
	using CoordinateList = std::vector<TIndex>;
	
	struct Node
	{
		//G 현재까지 누적거리
		//H 목표까지 최단거리
		uint G, H;
		//좌표
		TIndex coordinates;
		//부모 노드
		Node *parent;
	
		Node(TIndex coord_, Node *parent_ = nullptr);
		//G + H = F 총비용
		uint getScore();
	};
	//노드 리스트
	using NodeSet = std::set<Node*>;
	
	class Generator
	{
		bool detectCollision(TIndex coordinates_);
		Node* findNodeOnList(NodeSet& nodes_, TIndex coordinates_);
		void releaseNodes(NodeSet& nodes_);
	
	public:
		Generator();
		void setWorldSize(TIndex worldSize_);
		void setDiagonalMovement(bool enable_);
		void setHeuristic(HeuristicFunction heuristic_);
		CoordinateList findPath(TIndex source_, TIndex target_);
		void addCollision(TIndex coordinates_);
		void removeCollision(TIndex coordinates_);
		void clearCollisions();
	
	private:
		HeuristicFunction heuristic;
		//방향, 벽
		CoordinateList direction, walls;
		TIndex worldSize;
		uint directions;
	};
	
	class Heuristic
	{
		static TIndex getDelta(TIndex source_, TIndex target_);
	
	public:
		static uint manhattan(TIndex source_, TIndex target_);//맨헤튼
		static uint euclidean(TIndex source_, TIndex target_);//유클리드
		static uint octagonal(TIndex source_, TIndex target_);//8변
	};

};

