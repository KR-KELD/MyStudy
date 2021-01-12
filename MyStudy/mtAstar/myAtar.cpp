#include "myAtar.h"
#include <algorithm>
using namespace std::placeholders;

//std::bind(�Լ� �ּ�, �Լ�����1, �Լ�����2, �Լ�����3, ...)
//std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ... ���� ���ο� �Լ� ���ڿ� ���� ��ų���ִ�.
//void Func(int iA, float fB, bool bC);
//auto funcA = std::bind(Func, 10, 2.0f, true);funcA();
//auto funcB = std::bind(Func, 10, std::placeholders::_1, true);funcB(2.0f);
//auto funcC = std::bind(Func, std::placeholders::_2, std::placeholders::_1, true);funcC(2.0f, 10);
//auto funcD = std::bind(Func, std::placeholders::_2, std::placeholders::_1, std::placeholders::_3);funcD(2.0f, 10, true);
//�׸��� �����Լ��� �ƴ� Ŭ���� �Լ��� ���
//&ClassA::FuncA ���¸� ���ε��� ù���� ���ڷ� ����Ѵ�


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
//�⺻ ����
AStar::Generator::Generator()
{
	setDiagonalMovement(false);
	setHeuristic(&Heuristic::manhattan);
	direction = {
		{ 0, 1 }, // ��
	{ 1, 0 }, // ��
	{ 0, -1 },// ��
	{ -1, 0 },// ��
	{ -1, -1 },{ 1, 1 },{ -1, 1 },{ 1, -1 }
	};
}
//���� ������ ����
void AStar::Generator::setWorldSize(TIndex worldSize_)
{
	worldSize = worldSize_;
}

//4�� 8�� ����
void AStar::Generator::setDiagonalMovement(bool enable_)
{
	directions = (enable_ ? 8 : 4);
}
//�Ÿ����� ����
void AStar::Generator::setHeuristic(HeuristicFunction heuristic_)
{
	heuristic = std::bind(heuristic_, _1, _2);
}
//�� ����Ʈ�� �ε��� �߰�
void AStar::Generator::addCollision(TIndex coordinates_)
{
	walls.push_back(coordinates_);
}
//�� ����Ʈ�� �ε��� ����
void AStar::Generator::removeCollision(TIndex coordinates_)
{
	auto it = std::find(walls.begin(), walls.end(), coordinates_);
	if (it != walls.end())
	{
		walls.erase(it);
	}
}
//�� ��� ����
void AStar::Generator::clearCollisions()
{
	walls.clear();
}

AStar::CoordinateList AStar::Generator::findPath(TIndex source_, TIndex target_)
{
	//���� ���
	Node *current = nullptr;
	//����,Ŭ���� ��� ����Ʈ
	NodeSet openSet, closedSet;
	//���� ���� ����
	openSet.insert(new Node(source_));
	//
	std::vector<TIndex> currentList;
	//���³�� �˻�
	while (!openSet.empty())
	{
		//���³�带 ��ȸ
		current = *openSet.begin();
		//���³�� ����Ʈ���� F���� ���� ���� ��带 ã�´�
		for (auto node : openSet)
		{
			int iNodeValue = node->getScore();
			int iCurrentValue = current->getScore();
			if (iNodeValue <= iCurrentValue)
			{
				current = node;
			}
		}
		//�������� ã�Ҵٸ� �����
		if (current->coordinates == target_)
		{
			break;
		}
		//Ž�� ���� ����Ʈ�� ���� ��ǥ�� ��´�
		currentList.push_back(current->coordinates);
		//���� ��带 �ݴ´�
		closedSet.insert(current);
		//���� ����Ʈ���� ���� ��带 ����
		openSet.erase(std::find(openSet.begin(), openSet.end(), current));
		//���� Ž��
		for (uint i = 0; i < directions; ++i)
		{
			//������ �� �ִ� ���� Ž��
			TIndex newCoordinates(current->coordinates + direction[i]);
			//���� ���� ���� �ǳʶٱ�
			if (detectCollision(newCoordinates))
			{
				continue;
			}
			//Ŭ���� ����Ʈ�� ����
			if (findNodeOnList(closedSet, newCoordinates))
			{
				continue;
			}
			//�밢�� 14 ���� 10
			//���� �̵������ �ڽ�Ʈ�� ���
			uint totalCost = current->G + ((i < 4) ? 10 : 14);
			//uint totalCost = current->G + ((i < 4) ? 1 : 2);
			//���ο� ������ ��带 ���¸���Ʈ���� �˻�
			Node *successor = findNodeOnList(openSet, newCoordinates);
			//�ȵ�������� �����ض�
			if (successor == nullptr)
			{
				successor = new Node(newCoordinates, current);
				successor->G = totalCost;
				successor->H = heuristic(successor->coordinates, target_);
				openSet.insert(successor);
			}
			//���� ������ �ִ� G���� ���� G������ ũ�ٸ�
			//�� ���� ����� �θ� ���� ���� �ٲٰ�
			//G���� �����Ѵ�
			else if (totalCost < successor->G)
			{
				successor->parent = current;
				successor->G = totalCost;
			}
		}
	}

	CoordinateList path;
	//���� ������ �������� ��ȸ�ϸ鼭
	//���������� ã�� ����Ʈ�� �����Ѵ�
	while (current != nullptr)
	{
		path.push_back(current->coordinates);
		current = current->parent;
	}

	releaseNodes(openSet);
	releaseNodes(closedSet);

	return path;
}
//����Ʈ �� ��� �˻�
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
//����Ʈ �� ��� ����
void AStar::Generator::releaseNodes(NodeSet& nodes_)
{
	for (auto it = nodes_.begin(); it != nodes_.end();)
	{
		delete *it;
		it = nodes_.erase(it);
	}
}

//���� ����� ������ üũ(���� �����ڸ�, ������)
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
//x,y �Ÿ� ��� �� ����
TIndex AStar::Heuristic::getDelta(TIndex source_, TIndex target_)
{
	return{ abs(source_.x - target_.x),  abs(source_.y - target_.y) };
}
//�Ÿ� ��� ��
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