#include "myAtar.h"
#include <algorithm>
using namespace std::placeholders;

//std::bind(함수 주소, 함수인자1, 함수인자2, 함수인자3, ...)
//std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ... 등을 새로운 함수 인자와 맵핑 시킬수있다.
//void Func(int iA, float fB, bool bC);
//auto funcA = std::bind(Func, 10, 2.0f, true);funcA();
//auto funcB = std::bind(Func, 10, std::placeholders::_1, true);funcB(2.0f);
//auto funcC = std::bind(Func, std::placeholders::_2, std::placeholders::_1, true);funcC(2.0f, 10);
//auto funcD = std::bind(Func, std::placeholders::_2, std::placeholders::_1, std::placeholders::_3);funcD(2.0f, 10, true);
//그리고 전역함수가 아닌 클래스 함수의 경우
//&ClassA::FuncA 행태를 바인드의 첫번쩨 인자로 사용한다


AStar::Node::Node(TIndex coordinates_, Node *parent_)
{
	parent = parent_;
	coordinates = coordinates_;
	G = H = 0;
}

AStar::uint AStar::Node::getScore()
{
	return G + H;
}
//기본 세팅
AStar::Generator::Generator()
{
	setDiagonalMovement(false);
	setHeuristic(&Heuristic::manhattan);
	direction = {
		{ 0, 1 }, // 북
	{ 1, 0 }, // 동
	{ 0, -1 },// 서
	{ -1, 0 },// 남
	{ -1, -1 },{ 1, 1 },{ -1, 1 },{ 1, -1 }
	};
}
//월드 사이즈 결정
void AStar::Generator::setWorldSize(TIndex worldSize_)
{
	worldSize = worldSize_;
}

//4방 8방 결정
void AStar::Generator::setDiagonalMovement(bool enable_)
{
	directions = (enable_ ? 8 : 4);
}
//거리계산식 결정
void AStar::Generator::setHeuristic(HeuristicFunction heuristic_)
{
	heuristic = std::bind(heuristic_, _1, _2);
}
//벽 리스트에 인덱스 추가
void AStar::Generator::addCollision(TIndex coordinates_)
{
	walls.push_back(coordinates_);
}
//벽 리스트에 인덱스 제거
void AStar::Generator::removeCollision(TIndex coordinates_)
{
	auto it = std::find(walls.begin(), walls.end(), coordinates_);
	if (it != walls.end())
	{
		walls.erase(it);
	}
}
//벽 모두 제거
void AStar::Generator::clearCollisions()
{
	walls.clear();
}

AStar::CoordinateList AStar::Generator::findPath(TIndex source_, TIndex target_)
{
	//현재 노드
	Node *current = nullptr;
	//오픈,클로즈 노드 리스트
	NodeSet openSet, closedSet;
	//현재 지점 오픈
	openSet.insert(new Node(source_));
	//
	std::vector<TIndex> currentList;
	//오픈노드 검사
	while (!openSet.empty())
	{
		//오픈노드를 순회
		current = *openSet.begin();
		//오픈노드 리스트에서 F값이 가장 낮은 노드를 찾는다
		for (auto node : openSet)
		{
			int iNodeValue = node->getScore();
			int iCurrentValue = current->getScore();
			if (iNodeValue <= iCurrentValue)
			{
				current = node;
			}
		}
		//목적지를 찾았다면 멈춰라
		if (current->coordinates == target_)
		{
			break;
		}
		//탐색 과정 리스트에 현재 좌표를 담는다
		currentList.push_back(current->coordinates);
		//현재 노드를 닫는다
		closedSet.insert(current);
		//오픈 리스트에서 현재 노드를 뺀다
		openSet.erase(std::find(openSet.begin(), openSet.end(), current));
		//방향 탐색
		for (uint i = 0; i < directions; ++i)
		{
			//움직일 수 있는 방향 탐색
			TIndex newCoordinates(current->coordinates + direction[i]);
			//벽과 지도 끝은 건너뛰기
			if (detectCollision(newCoordinates))
			{
				continue;
			}
			//클로즈 리스트는 빼기
			if (findNodeOnList(closedSet, newCoordinates))
			{
				continue;
			}
			//대각선 14 직선 10
			//다음 이동경로의 코스트를 계산
			uint totalCost = current->G + ((i < 4) ? 10 : 14);
			//uint totalCost = current->G + ((i < 4) ? 1 : 2);
			//새로운 방향의 노드를 오픈리스트에서 검색
			Node *successor = findNodeOnList(openSet, newCoordinates);
			//안들어있으면 세팅해라
			if (successor == nullptr)
			{
				successor = new Node(newCoordinates, current);
				successor->G = totalCost;
				successor->H = heuristic(successor->coordinates, target_);
				openSet.insert(successor);
			}
			//원래 가지고 있던 G값이 현재 G값보다 크다면
			//그 방향 노드의 부모를 현재 노드로 바꾸고
			//G값을 갱신한다
			else if (totalCost < successor->G)
			{
				successor->parent = current;
				successor->G = totalCost;
			}
		}
	}

	CoordinateList path;
	//현재 노드부터 역순으로 순회하면서
	//최종적으로 찾은 리스트를 리턴한다
	while (current != nullptr)
	{
		path.push_back(current->coordinates);
		current = current->parent;
	}

	releaseNodes(openSet);
	releaseNodes(closedSet);

	return path;
}
//리스트 내 노드 검색
AStar::Node* AStar::Generator::findNodeOnList(NodeSet& nodes_, TIndex coordinates_)
{
	for (auto node : nodes_)
	{
		if (node->coordinates == coordinates_)
		{
			return node;
		}
	}
	return nullptr;
}
//리스트 내 노드 제거
void AStar::Generator::releaseNodes(NodeSet& nodes_)
{
	for (auto it = nodes_.begin(); it != nodes_.end();)
	{
		delete *it;
		it = nodes_.erase(it);
	}
}

//현재 노드의 벽상태 체크(맵의 가장자리, 벽상태)
bool AStar::Generator::detectCollision(TIndex coordinates_)
{
	if (coordinates_.x < 0 || coordinates_.x >= worldSize.x ||
		coordinates_.y < 0 || coordinates_.y >= worldSize.y ||
		std::find(walls.begin(), walls.end(), coordinates_) != walls.end())
	{
		return true;
	}
	return false;
}
//x,y 거리 계산 식 과정
TIndex AStar::Heuristic::getDelta(TIndex source_, TIndex target_)
{
	return{ abs(source_.x - target_.x),  abs(source_.y - target_.y) };
}
//거리 계산 식
AStar::uint AStar::Heuristic::manhattan(TIndex source_, TIndex target_)
{
	auto delta = std::move(getDelta(source_, target_));
	return static_cast<uint>(10 * (delta.x + delta.y));
}

AStar::uint AStar::Heuristic::euclidean(TIndex source_, TIndex target_)
{
	TIndex a = getDelta(source_, target_);
	TIndex&& delta = std::move(getDelta(source_, target_));
	return static_cast<uint>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
}

AStar::uint AStar::Heuristic::octagonal(TIndex source_, TIndex target_)
{
	auto delta = std::move(getDelta(source_, target_));
	return 10 * (delta.x + delta.y) + (-6) * min(delta.x, delta.y);
}