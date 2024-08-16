#include "GridSystem.h"
#include "MyWindow.h"
void GridSystem::JPSearch(List<std::pair<float, float>>& pathPoints)
{
    Node* pStartNode = _nodePool.Alloc();
    //Node* pStartNode = new Node;
    _allocatedNodes.push_back(pStartNode);

    pStartNode->x = _startX;
    pStartNode->y = _startY;
    pStartNode->moveLen = 0;
    pStartNode->destLen = sqrt(pow(abs(pStartNode->x - _endX), 2) + pow(abs(pStartNode->y - _endY), 2));
    _closeList[std::make_pair(pStartNode->x, pStartNode->y)] = pStartNode;

    SearchRR(_startX, _startY, pStartNode);
    SearchRD(pStartNode);
    SearchDD(_startX, _startY, pStartNode);
    SearchLD(pStartNode);
    SearchLL(_startX, _startY, pStartNode);
    SearchLU(pStartNode);
    SearchUU(_startX, _startY, pStartNode);
    SearchRU(pStartNode);

    while (1)
    {
        if (_openList.empty())
        {
            break;
        }
        Node* pCurrentNode = *_openList.begin(); 
        _openList.erase(_openList.begin());

        if (pCurrentNode->x == _endX && pCurrentNode->y == _endY)
        {
            while (1)
            {
                pathPoints.push_front(std::make_pair( pCurrentNode->x * _cellSize - _cellSize/2, pCurrentNode->y*_cellSize - _cellSize/2));
                if (pCurrentNode->x == _startX && pCurrentNode->y == _startY)
                {
                    break;
                }
                Node* pPrevNode = pCurrentNode->pParent;
                while (pPrevNode->pParent && CheckObstacleOnLineByBresenham(pPrevNode->pParent->x, pPrevNode->pParent->y, pCurrentNode->x, pCurrentNode->y))
                {
                    pPrevNode = pPrevNode->pParent;
                }
                pCurrentNode->pParent = pPrevNode;
                pCurrentNode = pPrevNode;
            }
            break;
        }

        int difX = pCurrentNode->x - pCurrentNode->pParent->x;
        int difY = pCurrentNode->y - pCurrentNode->pParent->y;
        if (difX > 0 && difY == 0)
        {
            //RR
            SearchRR(pCurrentNode->x, pCurrentNode->y, pCurrentNode);
            if (_obstacleMap[pCurrentNode->y + 1][pCurrentNode->x] == true)
            {
                SearchRU(pCurrentNode);
            }
            if (_obstacleMap[pCurrentNode->y - 1][pCurrentNode->x] == true)
            {
                SearchRD(pCurrentNode);
            }
        }
        else if (difX > 0 && difY > 0)
        {
            //RU
            SearchRR(pCurrentNode->x, pCurrentNode->y, pCurrentNode);
            SearchRU(pCurrentNode);
            SearchUU(pCurrentNode->x, pCurrentNode->y, pCurrentNode);
            if (_obstacleMap[pCurrentNode->y - 1][pCurrentNode->x] == true)
            {
                SearchRD(pCurrentNode);
            }
            if (_obstacleMap[pCurrentNode->y][pCurrentNode->x - 1] == true)
            {
                SearchLU(pCurrentNode);
            }
        }
        else if (difX == 0 && difY > 0)
        {
            //UU
            SearchUU(pCurrentNode->x, pCurrentNode->y, pCurrentNode);
            if (_obstacleMap[pCurrentNode->y][pCurrentNode->x + 1] == true)
            {
                SearchRU(pCurrentNode);
            }
            if (_obstacleMap[pCurrentNode->y][pCurrentNode->x - 1] == true)
            {
                SearchLU(pCurrentNode);
            }
        }
        else if (difX < 0 && difY > 0)
        {
            //LU
            SearchLL(pCurrentNode->x, pCurrentNode->y, pCurrentNode);
            SearchLU(pCurrentNode);
            SearchUU(pCurrentNode->x, pCurrentNode->y, pCurrentNode);
            if (_obstacleMap[pCurrentNode->y][pCurrentNode->x + 1] == true)
            {
                SearchRU(pCurrentNode);
            }
            if (_obstacleMap[pCurrentNode->y - 1][pCurrentNode->x] == true)
            {
                SearchLD(pCurrentNode);
            }
        }
        else if (difX < 0 && difY == 0)
        {
            //LL
            SearchLL(pCurrentNode->x, pCurrentNode->y, pCurrentNode);
            if (_obstacleMap[pCurrentNode->y + 1][pCurrentNode->x] == true)
            {
                SearchLU(pCurrentNode);
            }
            if (_obstacleMap[pCurrentNode->y - 1][pCurrentNode->x] == true)
            {
                SearchLD(pCurrentNode);
            }
        }
        else if (difX < 0 && difY < 0)
        {
            //LD
            SearchLL(pCurrentNode->x, pCurrentNode->y, pCurrentNode);
            SearchLD(pCurrentNode);
            SearchDD(pCurrentNode->x, pCurrentNode->y, pCurrentNode);
            if (_obstacleMap[pCurrentNode->y + 1][pCurrentNode->x] == true)
            {
                SearchLU(pCurrentNode);
            }
            if (_obstacleMap[pCurrentNode->y][pCurrentNode->x + 1] == true)
            {
                SearchRD(pCurrentNode);
            }
        }
        else if (difX == 0 && difY < 0)
        {
            //DD
            SearchDD(pCurrentNode->x, pCurrentNode->y, pCurrentNode);
            if (_obstacleMap[pCurrentNode->y][pCurrentNode->x - 1] == true)
            {
                SearchLD(pCurrentNode);
            }
            if (_obstacleMap[pCurrentNode->y][pCurrentNode->x + 1] == true)
            {
                SearchRD(pCurrentNode);
            }
        }
        else if (difX > 0 && difY < 0)
        {
            //RD
            SearchRR(pCurrentNode->x, pCurrentNode->y, pCurrentNode);
            SearchRD(pCurrentNode);
            SearchDD(pCurrentNode->x, pCurrentNode->y, pCurrentNode);
            if (_obstacleMap[pCurrentNode->y + 1][pCurrentNode->x] == true)
            {
                SearchRU(pCurrentNode);
            }
            if (_obstacleMap[pCurrentNode->y][pCurrentNode->x - 1] == true)
            {
                SearchLD(pCurrentNode);
            }
        }
    }
}
void GridSystem::AStar(List<std::pair<float, float>>& pathPoints)
{
    //Node* pStartNode = new Node;
    Node* pStartNode =  _nodePool.Alloc();
    _allocatedNodes.push_back(pStartNode);
    
    pStartNode->x = _startX;
    pStartNode->y = _startY;
    pStartNode->moveLen = 0;
    pStartNode->destLen = sqrt(pow(abs(pStartNode->x - _endX), 2) + pow(abs(pStartNode->y - _endY), 2));

    _openList.insert(pStartNode);
    _closeList[std::make_pair(_startX, _startY)] = pStartNode;
    while (1)
    {
        if (_openList.empty())
        {
            break;
        }
        Node* pCurrentNode = *_openList.begin(); 
        _openList.erase(_openList.begin());
        if (pCurrentNode->x == _endX && pCurrentNode->y == _endY)
        {
            while (1)
            {
                pathPoints.push_front(std::make_pair(pCurrentNode->x * _cellSize - _cellSize / 2, pCurrentNode->y * _cellSize - _cellSize / 2));
                if (pCurrentNode->x == _startX && pCurrentNode->y == _startY)
                {
                    break;
                }
                pCurrentNode = pCurrentNode->pParent;
            }
            break;
        }

        for (int iY = -1; iY <= 1; iY++)
        {
            for (int iX = -1; iX <= 1; iX++)
            {
                if (iX == 0 && iY == 0)
                {
                    continue;
                }
                if (_obstacleMap[pCurrentNode->y + iY][pCurrentNode->x + iX] == false)
                {
                    UpdateLists(pCurrentNode->x + iX, pCurrentNode->y + iY, pCurrentNode);
                }
            }
        }
    }
    return;
}
void GridSystem::SetObstacle(int x, int y, bool bObstacle)
{
    _obstacleMap[y+1][x+1] = bObstacle;
}
bool GridSystem::IsObstacle(int x, int y)
{
    return _obstacleMap[y+1][x+1];
}
bool GridSystem::IsObstacleByFloat(float x, float y)
{
    int gridX = x / _cellSize+1;
    int gridY = y / _cellSize+1;
    return _obstacleMap[gridY][gridX];
}
bool GridSystem::GetNotObstacleLocation(std::pair<float, float>& location)
{
    bool ret=false;
    for (int iY = 1; iY <= _height; iY++)
    {
        for (int iX = 1; iX <= _width; iX++)
        {
            if (IsObstacle(iX, iY) == false)
            {
                ret = true;
                location.first = (iX - 1)*_cellSize+ _cellSize /2;
                location.second = (iY - 1)*_cellSize+ _cellSize/2;
            }
        }
        if (ret == true)
        {
            break;
        }
    }
    return ret;
}
bool GridSystem::GetNearNotObstacleLocation(float x, float y, std::pair<float, float>& location)
{
    bool ret = false;

    int gridX = x / _cellSize + 1;
    int gridY = y / _cellSize + 1;

    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            if (_obstacleMap[gridY+dy][gridX+dx] == false)
            {
                ret = true;
                location.first = (gridX+dx) * _cellSize - _cellSize / 2;
                location.second = (gridY+dy) * _cellSize - _cellSize / 2;
                break;
            }
        }
    }

    return ret;
}
bool GridSystem::FindPath(float startX, float startY, float endX, float endY, List<std::pair<float, float>>& pathPoints)
{
    pathPoints.clear();
    
    bool ret = true;
    _startX = startX / _cellSize + 1;
    _startY = startY / _cellSize + 1;
    _endX = endX / _cellSize + 1;
    _endY = endY / _cellSize + 1;
    
    if (_obstacleMap[_endY][_endX] == true)
    {
        return false;
    }

    if (_obstacleMap[_startY][_startX] == true)
    {
        // 멈추고 움직이는 프로토콜을 사용할 때 사용하는 코드
        /*std::pair<float, float> notObstacleLocation;
        if (GetNearNotObstacleLocation(startX, startY, notObstacleLocation)==true)
        {
            pathPoints.push_back(notObstacleLocation);
            return false;
        }

        GetNotObstacleLocation(notObstacleLocation);
        pathPoints.push_back(notObstacleLocation);*/
        return false;
    }

    if (_startX == _endX && _startY == _endY)
    {
        //JPSearch()함수가 시작과 끝이 같을때 결과를 뱉지 않음 예외처리
        pathPoints.push_back(std::make_pair(startX, startY));
        pathPoints.push_back(std::make_pair(endX, endY));

    }
    else
    {
        _openList.clear();
        _closeList.clear();
        JPSearch(pathPoints);
        if (pathPoints.size() < 2)
        {
            ret = false;
        }
        else
        {
            pathPoints.front().first = startX;
            pathPoints.front().second = startY;
            pathPoints.back().first = endX;
            pathPoints.back().second = endY;
        }
        FreeNodes();
    }
    return ret;
}
bool GridSystem::IsSameGrid(float x1, float y1, float x2, float y2)
{
    int gridX1 = x1 / _cellSize;
    int gridY1 = y1 / _cellSize;

    int gridX2 = x2 / _cellSize;
    int gridY2 = y2 / _cellSize;

    return (gridX1 == gridX2) && (gridY1 == gridY2);
}
bool GridSystem::SearchRR(int beginX, int beginY, Node* pParentNode, bool makeNode)
{
    int currentX = beginX;
    int currentY = beginY;
    while (1)
    {
        currentX += 1;
        if (_obstacleMap[currentY][currentX] == true)
        {
            break;
        }
        if (currentX == _endX && currentY == _endY)
        {
            if (makeNode)
            {
                UpdateLists(currentX, currentY, pParentNode);
            }
            return true;
        }
        if (_obstacleMap[currentY + 1][currentX] == true && _obstacleMap[currentY + 1][currentX + 1] == false)
        {
            if (makeNode)
            {
                UpdateLists(currentX, currentY, pParentNode);
            }
            return true;
        }
        if (_obstacleMap[currentY - 1][currentX] == true && _obstacleMap[currentY - 1][currentX + 1] == false)
        {
            if (makeNode)
            {
                UpdateLists(currentX, currentY, pParentNode);
            }
            return true;
        }

    }

    return false;
}
bool GridSystem::SearchDD(int beginX, int beginY, Node* pParentNode, bool makeNode)
{
    int currentX = beginX;
    int currentY = beginY;

    while (1)
    {
        currentY -= 1;
        if (_obstacleMap[currentY][currentX] == true)
        {
            break;
        }
        if (currentX == _endX && currentY == _endY)
        {
            if (makeNode)
            {
                UpdateLists(currentX, currentY, pParentNode);
            }
            return true;
        }
        if (_obstacleMap[currentY][currentX + 1] == true && _obstacleMap[currentY - 1][currentX + 1] == false)
        {
            if (makeNode)
            {
                UpdateLists(currentX, currentY, pParentNode);
            }
            return true;
        }
        if (_obstacleMap[currentY][currentX - 1] == true && _obstacleMap[currentY - 1][currentX - 1] == false)
        {
            if (makeNode)
            {
                UpdateLists(currentX, currentY, pParentNode);
            }
            return true;
        }

    }

    return false;
}
bool GridSystem::SearchLL(int beginX, int beginY, Node* pParentNode, bool makeNode)
{
    int currentX = beginX;
    int currentY = beginY;

    while (1)
    {
        currentX -= 1;
        if (_obstacleMap[currentY][currentX] == true)
        {
            break;
        }
        if (currentX == _endX && currentY == _endY)
        {
            if (makeNode)
            {
                UpdateLists(currentX, currentY, pParentNode);
            }
            return true;
        }
        if (_obstacleMap[currentY - 1][currentX] == true && _obstacleMap[currentY - 1][currentX - 1] == false)
        {
            if (makeNode)
            {
                UpdateLists(currentX, currentY, pParentNode);
            }
            return true;
        }
        if (_obstacleMap[currentY + 1][currentX] == true && _obstacleMap[currentY + 1][currentX - 1] == false)
        {
            if (makeNode)
            {
                UpdateLists(currentX, currentY, pParentNode);
            }
            return true;
        }

    }

    return false;
}
bool GridSystem::SearchUU(int beginX, int beginY, Node* pParentNode, bool makeNode)
{
    int currentX = beginX;
    int currentY = beginY;
    while (1)
    {
        currentY += 1;
        if (_obstacleMap[currentY][currentX] == true)
        {
            break;
        }
        if (currentX == _endX && currentY == _endY)
        {
            if (makeNode)
            {
                UpdateLists(currentX, currentY, pParentNode);
            }
            return true;
        }
        if (_obstacleMap[currentY][currentX - 1] == true && _obstacleMap[currentY + 1][currentX - 1] == false)
        {
            if (makeNode)
            {
                UpdateLists(currentX, currentY, pParentNode);
            }
            return true;
        }
        if (_obstacleMap[currentY][currentX + 1] == true && _obstacleMap[currentY + 1][currentX + 1] == false)
        {
            if (makeNode)
            {
                UpdateLists(currentX, currentY, pParentNode);
            }
            return true;
        }

    }

    return false;
}
void GridSystem::SearchRD(Node* pParentNode)
{
    int currentX = pParentNode->x;
    int currentY = pParentNode->y;

    while (1)
    {
        currentX += 1;
        currentY -= 1;
        if (_obstacleMap[currentY][currentX] == true)
        {
            break;
        }
        if (currentX == _endX && currentY == _endY)
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }
        if (_obstacleMap[currentY + 1][currentX] == true && _obstacleMap[currentY + 1][currentX + 1] == false)
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }
        if (_obstacleMap[currentY][currentX - 1] == true && _obstacleMap[currentY - 1][currentX - 1] == false)
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }
        if (SearchRR(currentX, currentY, pParentNode, false) || SearchDD(currentX, currentY, pParentNode, false))
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }

    }

    return;
}
void GridSystem::SearchLD(Node* pParentNode)
{
    int currentX = pParentNode->x;
    int currentY = pParentNode->y;

    while (1)
    {
        currentX -= 1;
        currentY -= 1;
        if (_obstacleMap[currentY][currentX] == true)
        {
            break;
        }
        if (currentX == _endX && currentY == _endY)
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }
        if (_obstacleMap[currentY + 1][currentX] == true && _obstacleMap[currentY + 1][currentX - 1] == false)
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }
        if (_obstacleMap[currentY][currentX + 1] == true && _obstacleMap[currentY - 1][currentX + 1] == false)
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }
        if (SearchLL(currentX, currentY, pParentNode, false) || SearchDD(currentX, currentY, pParentNode, false))
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }
    }
    return;
}
void GridSystem::SearchLU(Node* pParentNode)
{
    int currentX = pParentNode->x;
    int currentY = pParentNode->y;

    while (1)
    {
        currentX -= 1;
        currentY += 1;
        if (_obstacleMap[currentY][currentX] == true)
        {
            break;
        }
        if (currentX == _endX && currentY == _endY)
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }
        if (_obstacleMap[currentY - 1][currentX] == true && _obstacleMap[currentY - 1][currentX - 1] == false)
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }
        if (_obstacleMap[currentY][currentX + 1] == true && _obstacleMap[currentY + 1][currentX + 1] == false)
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }
        if (SearchLL(currentX, currentY, pParentNode, false) || SearchUU(currentX, currentY, pParentNode, false))
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }
    }
    return;
}
void GridSystem::SearchRU(Node* pParentNode)
{
    int currentX = pParentNode->x;
    int currentY = pParentNode->y;

    while (1)
    {
        currentX += 1;
        currentY += 1;
        if (_obstacleMap[currentY][currentX] == true)
        {
            break;
        }
        if (currentX == _endX && currentY == _endY)
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }
        if (_obstacleMap[currentY][currentX - 1] == true && _obstacleMap[currentY + 1][currentX - 1] == false)
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }
        if (_obstacleMap[currentY - 1][currentX] == true && _obstacleMap[currentY - 1][currentX + 1] == false)
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }
        if (SearchRR(currentX, currentY, pParentNode, false) || SearchUU(currentX, currentY, pParentNode, false))
        {
            UpdateLists(currentX, currentY, pParentNode);
            break;
        }
    }
    return;
}

GridSystem::Node* GridSystem::MakeNewNode(int x, int y, Node* pParentNode)
{
    Node* pNewNode = _nodePool.Alloc();
    //Node* pNewNode = new Node;
    _allocatedNodes.push_back(pNewNode);
    pNewNode->x = x;
    pNewNode->y = y;
    pNewNode->pParent = pParentNode;
    pNewNode->destLen = sqrt(pow(abs(pNewNode->x - _endX), 2) + pow(abs(pNewNode->y - _endY), 2));
    pNewNode->moveLen = pParentNode->moveLen + sqrt(pow(abs(x - pParentNode->x), 2) + pow(abs(y - pParentNode->y), 2));
    return pNewNode;
}
void GridSystem::UpdateLists(int currentX, int currentY, Node* pParentNode)
{
    auto pairXY = std::make_pair(currentX, currentY);
    if (_closeList.find(pairXY) == _closeList.end())
    {
        Node* pNewNode = MakeNewNode(currentX, currentY, pParentNode);
        _openList.insert(pNewNode);
        _closeList[pairXY] = pNewNode;
    }
    else
    {
        Node* pRevisitNode = _closeList[pairXY];
        double prevVal = pRevisitNode->destLen + pRevisitNode->moveLen;
        double destLen = sqrt(pow(abs(currentX - _endX), 2) + pow(abs(currentY - _endY), 2));
        double moveLen = pParentNode->moveLen + sqrt(pow(abs(currentX - pParentNode->x), 2) + pow(abs(currentY - pParentNode->y), 2));
        if (prevVal > destLen + moveLen)
        {
            _openList.erase(pRevisitNode);
            pRevisitNode->destLen = destLen;
            pRevisitNode->moveLen = moveLen;
            pRevisitNode->pParent = pParentNode;
            _openList.insert(pRevisitNode);
        }
    }
}
bool GridSystem::CheckObstacleOnLine(double beginX, double beginY, double endX, double endY)
{
    int lowX = min(beginX, endX);
    int highX = max(beginX, endX);
    int lowY = min(beginY, endY);
    int highY = max(beginY, endY);
    for (int iX = lowX + 1; iX <= highX; iX++)
    {
        double y = ((endY - beginY) / (endX - beginX)) * (iX - beginX) + beginY;
        int iY = y;
        if (y != iY)
        {
            if (_obstacleMap[iY][iX] || _obstacleMap[iY][iX - 1])
            {
                return false;
            }
        }
        //현재 조건으로는 격자점 처리를 안해도 될 듯
    }
    for (int iY = lowY + 1; iY <= highY; iY++)
    {
        double x = ((endX - beginX) / (endY - beginY)) * (iY - beginY) + beginX;
        int iX = x;
        if (x != iX)
        {
            if (_obstacleMap[iY][iX] || _obstacleMap[iY - 1][iX])
            {
                return false;
            }
        }
    }
    return true;
}

GridSystem::GridSystem(int height, int width, float cellSize)
{
    _height = height;
    _width = width;
    _cellSize = cellSize;
    _obstacleMap = new bool* [height+2];
    for (int i = 0; i < height+2; i++)
    {
        _obstacleMap[i] = new bool[width+2];
        for (int j = 0; j < width+2; j++)
        {
            _obstacleMap[i][j] = false;
            if (i == 0 || j == 0 || i== height + 1 || j == width + 1)
            {
                _obstacleMap[i][j] = true;
            }
        }

    }
}
GridSystem::~GridSystem()
{
    for (int i = 0; i < _height + 2; i++)
    {
        delete[] _obstacleMap[i];
    }
    delete[] _obstacleMap;
}
bool GridSystem::CheckObstacleOnLineByBresenham(int beginX, int beginY, int endX, int endY)
{
    int difX = abs(beginX - endX);
    int difY = abs(beginY - endY);
    int deltaX;
    int deltaY;
    if (beginX < endX)
    {
        deltaX = 1;
    }
    else
    {
        deltaX = -1;
    }

    if (beginY < endY)
    {
        deltaY = 1;
    }
    else
    {
        deltaY = -1;
    }
    int sum = 0;
    if (difX > difY)
    {

        sum += difX / 2;
        int iY = beginY;
        for (int iX = beginX; iX != (int)endX; iX += deltaX)
        {
            if (sum >= difX)
            {
                sum -= difX;
                iY += deltaY;
            }
            if (_obstacleMap[iY][iX] == true)
            {
                return false;
            }
            sum += difY;

        }
    }
    else
    {
        sum += difY / 2;
        int iX = beginX;
        for (int iY = beginY; iY != (int)endY; iY += deltaY)
        {
            if (sum >= difY)
            {
                sum -= difY;
                iX += deltaX;
            }
            if (_obstacleMap[iY][iX] == true)
            {
                return false;
            }
            sum += difX;
        }
    }
    return true;
}
void GridSystem::FreeNodes()
{
    for (Node* pNode : _allocatedNodes)
    {
        //delete pNode;
        _nodePool.Free(pNode);
    }
    _allocatedNodes.clear();
}

