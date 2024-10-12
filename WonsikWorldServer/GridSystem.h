#pragma once
#include "Container/MyStlContainer.h"
#include "Memory/TlsObjectPool.h"
class GridSystem
{
private:

	struct Node
	{
        int x=0;
        int y=0;
        double moveLen=0;
        double destLen=0;
        Node* pParent = nullptr;
        int id;
        bool  obstacle = false;
        inline static int newNodeId = 0;
        Node()
        {
            id = newNodeId++;
        }
	};
    struct cmp {
        bool operator()(Node* a, Node* b) const {

            if (a->moveLen + a->destLen == b->moveLen + b->destLen)
            {
                return a->id < b->id;
            }
            else
            {
                return a->moveLen + a->destLen < b->moveLen + b->destLen;
            }

        }
    };
    bool SearchRR(int beginX, int beginY, Node* pParentNode, bool makeNode = true);
    bool SearchDD(int beginX, int beginY, Node* pParentNode, bool makeNode = true);
    bool SearchLL(int beginX, int beginY, Node* pParentNode, bool makeNode = true);
    bool SearchUU(int beginX, int beginY, Node* pParentNode, bool makeNode = true);
    void SearchRD(Node* pParentNode); 
    void SearchLD(Node* pParentNode);
    void SearchLU(Node* pParentNode);
    void SearchRU(Node* pParentNode);

    Node* MakeNewNode(int x, int y, Node* pParentNode);
    void UpdateLists(int currentX, int currentY, Node* pParentNode);
    bool CheckObstacleOnLine(double beginX, double beginY, double endX, double endY);
    bool CheckObstacleOnLineByBresenham(int beginX, int beginY, int endX, int endY);
    void FreeNodes();
private:
    void JPSearch(List<std::pair<float, float>>& pathPoints);
    void AStar(List<std::pair<float, float>>& pathPoints);
    bool** _obstacleMap;
    TlsObjectPool<Node,true> _nodePool;
private:
    Set<Node*, cmp> _openSet;
    Set<std::pair<int, int>> _closeSet;
    Map<std::pair<int, int>, Node*> _costMap;
    inline static float _cellSize;
    int _height;
    int _width;
    int _startX=0;
    int _startY=0;
    int _endX=0;
    int _endY=0;
public:
    GridSystem(int height, int width, float cellSize);
    ~GridSystem();
    void SetObstacle(int x, int y, bool bObstacle);
    bool IsObstacle(int x, int y);
    bool IsObstacleByFloat(float x, float y);
    bool GetNotObstacleLocation(std::pair<float, float>& location);
    bool GetNearNotObstacleLocation(float x, float y, std::pair<float, float>& location);
    bool FindPath(float startX, float startY, float endX, float endY, List<std::pair<float,float>>& pathPoints);
    static bool IsSameGrid(float x1, float y1, float x2, float y2);
};