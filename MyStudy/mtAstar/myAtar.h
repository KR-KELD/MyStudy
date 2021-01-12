#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <set>
#include <Windows.h>

//�ε��� ����ü
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
	//�Լ�������, �Ÿ���� �Լ� ����
	using HeuristicFunction = std::function<uint(TIndex, TIndex)>;
	//��ǥ ����Ʈ
	using CoordinateList = std::vector<TIndex>;
	
	struct Node
	{
		//G ������� �����Ÿ�
		//H ��ǥ���� �ִܰŸ�
		uint G, H;
		//��ǥ
		TIndex coordinates;
		//�θ� ���
		Node *parent;
	
		Node(TIndex coord_, Node *parent_ = nullptr);
		//G + H = F �Ѻ��
		uint getScore();
	};
	//��� ����Ʈ
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
		//����, ��
		CoordinateList direction, walls;
		TIndex worldSize;
		uint directions;
	};
	
	class Heuristic
	{
		static TIndex getDelta(TIndex source_, TIndex target_);
	
	public:
		static uint manhattan(TIndex source_, TIndex target_);//����ư
		static uint euclidean(TIndex source_, TIndex target_);//��Ŭ����
		static uint octagonal(TIndex source_, TIndex target_);//8��
	};

};

