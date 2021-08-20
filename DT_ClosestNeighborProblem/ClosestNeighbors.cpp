#include "ClosestNeighbors.h"

ClosestNeighbors::ClosestNeighbors(const Settings& settings, const int countEdge)
{
	sqDOV = settings.SqDistanceView;
	sectorAngle = settings.sectorAngle;
	originPolygon = CreateOriginPolygon(countEdge);
}

std::map<size_t, indexArr> ClosestNeighbors::FindNeighborsBrute(const std::vector<UnitInfo>& dataUnit)
{
	auto sqDistance = [](int x, int y) { return x*x + y*y; };
	std::map<size_t, indexArr> neighbors;
	UnitInfo uM;
	UnitInfo uIt;

	// Поиск соседей
	//O(n^2)->O(n^2/2)
	clock_t tStartCalculateBrute1 = clock();
	for (int i = 0; i < dataUnit.size(); i++)
	{
		uM = dataUnit[i];
		for (int j = i+1;  j< dataUnit.size(); j++)
		{

			uIt = dataUnit[j];
			if (sqDistance(uM.posX - uIt.posX, uM.posY - uIt.posY) < sqDOV)
			{
				neighbors[i].push_back(j);
				neighbors[j].push_back(i);
			}
		}
	}
	std::cout << "CalculteBrute1Time: " << (clock() - (double)tStartCalculateBrute1) / CLOCKS_PER_SEC << std::endl;
	// Отбор по области видимости
	//O(3nN))
	std::vector<Edge> rotatingPolygon;
	clock_t tStartCalculateBrute2 = clock();
	for (int i = 0; i < neighbors.size(); i++)
	{
		//O(2*N));
		rotatingPolygon = RotatePolygon(dataUnit[i].directX, dataUnit[i].directY,dataUnit[i]);
		//O(2p[i]N));
		neighbors[i] = FindClosestPointInPolygon(rotatingPolygon, dataUnit, neighbors[i]);
	}
	std::cout << "CalculteBrute2Time: " << (clock() - (double)tStartCalculateBrute2) / CLOCKS_PER_SEC << std::endl;
	return neighbors;
}
std::map<size_t, indexArr> ClosestNeighbors::FindNeighborsKDTree(const std::vector<UnitInfo>& dataUnit)
{
	//Строим  O(nlogn)
	//std::vector<std::vector<int>> points;
	KDTree kdTree(dataUnit);
	std::map<size_t, indexArr> neighbors;

	// Поиск соседей
	//O(n^2)->O(n^2/2)
	clock_t tStartCalculateKDTree1 = clock();
	indexArr neighborsUnit;
	for (int i = 0; i < dataUnit.size(); i++)
	{
		neighbors[i] = kdTree.neighborhood_indices_Opt(std::vector<double>{(double)dataUnit[i].posX, (double)dataUnit[i].posY},sqDOV);

	}

	std::cout << "CalculteBruteKDTree1Time: " << (clock() - (double)tStartCalculateKDTree1) / CLOCKS_PER_SEC << std::endl;
	std::vector<Edge> rotatingPolygon;
	clock_t tStartCalculateKDTree2 = clock();
	for (int i = 0; i < neighbors.size(); i++)
	{
		//O(2*N));
		rotatingPolygon = RotatePolygon(dataUnit[i].directX, dataUnit[i].directY, dataUnit[i]);
		//O(2p[i]N));
		neighbors[i] = FindClosestPointInPolygon(rotatingPolygon, dataUnit, neighbors[i]);
	}
	std::cout << "CalculteKDTree2Time: " << (clock() - (double)tStartCalculateKDTree2) / CLOCKS_PER_SEC << std::endl;
	return neighbors;
}
std::vector<Edge> ClosestNeighbors::CreateOriginPolygon(const int countEdge)
{
	std::vector<Edge> polygon;
	SDL_FPoint* points = new SDL_FPoint[countEdge + 2];
	SDL_FPoint r0{ 0,0 };
	SDL_FPoint d{ 1, 0};
	float dov = sqrtf((float)sqDOV);
	float sectorAngleRadians = sectorAngle * D2R;
	float s = 0;
	float c = 0;
	points[0] = r0;
	//!
	for (int i = -countEdge / 2; i < 0; i++)
	{
		s = sinf(sectorAngleRadians * i / countEdge);
		c = cosf(sectorAngleRadians * i / countEdge);
		points[i + countEdge / 2 + 1].x = (r0.x + dov * (d.x * c - d.y * s));
		points[i + countEdge / 2 + 1].y = (r0.y + dov * (d.y * c + d.x * s));
	}
	if (countEdge % 2 != 0)
	{

		points[countEdge / 2 + countEdge % 2].x = (r0.x + dov * d.x);
		points[countEdge / 2 + countEdge % 2].y = (r0.y + dov * d.y);
	}
	for (int i = 1; i <= countEdge / 2; i++)
	{
		s =  sinf(sectorAngleRadians * i / countEdge);
		c = cosf(sectorAngleRadians * i / countEdge);
		points[i + countEdge / 2 + countEdge % 2].x = (r0.x + dov * (d.x * c - d.y * s));
		points[i + countEdge / 2 + countEdge % 2].y = (r0.y + dov * (d.y * c + d.x * s));
	}
	points[countEdge + 1] = r0;
	for (int i = 0; i <= countEdge + 1; i++)
	{
		polygon.push_back(
			{
				points[i].x,
				points[i].y,
			}
		);
	}
	delete[] points;
	points = nullptr;
	return polygon;
}

std::vector<Edge> ClosestNeighbors::RotatePolygon(const float sin, const float cos,const UnitInfo& unitInfo)
{
	std::vector<Edge> rotatingPolygon;
	for (const auto& edje : originPolygon)
	{
		rotatingPolygon.push_back({
			unitInfo.posX + edje.x0 * cos - edje.y0 * sin,
			unitInfo.posY + edje.x0 * sin + edje.y0 * cos
			});
	}
	return rotatingPolygon;
}

bool ClosestNeighbors::inside_convex_polygon(const UnitInfo& unitInfo, const std::vector<Edge>& polygon)
{
	int previous_side = 0;
	//int
	size_t n_vertices = polygon.size();
	Edge a;
	Edge b;
	Edge affine_segment;
	Edge affine_point;
	int current_side;
	Edge point{ (float)unitInfo.posX,(float)unitInfo.posY };
	for (int i = 0; i < n_vertices - 1; i++)
	{
		a = polygon[i];
		b = polygon[(i + 1) % n_vertices];
		affine_segment = v_sub(b, a);
		affine_point = v_sub(point, a);
		current_side = get_side(affine_segment, affine_point);
		if (current_side == 0)
		{
			return false;
		}
		else
		{
			if (previous_side ==0)
			{
				previous_side = current_side;
			}
			else
			{
				if (previous_side != current_side)
				{
					return false;
				}
			}
		}
	}
	return true;
}
int ClosestNeighbors::get_side(const Edge& a, const Edge& b)
{
	float x = cosine_sign(a, b);
	if (abs(x) < eps)
	{
		return 0;
	}
	if (x < 0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}
float ClosestNeighbors::cosine_sign(const Edge& a, const Edge& b)
{
	return a.x0 * b.y0 - a.y0 * b.x0;
}

Edge ClosestNeighbors::v_sub(const Edge& a, const Edge& b)
{
	return Edge{ a.x0 - b.x0, a.y0 - b.y0 };
}

indexArr ClosestNeighbors::FindClosestPointInPolygon(const std::vector<Edge>& polygon, const std::vector<UnitInfo>& dataUnit, const indexArr& neigbors)
{
	indexArr closesPoint;
	for (size_t k = 0; k < neigbors.size(); k++)
	{
		if (inside_convex_polygon(dataUnit[neigbors[k]],polygon))
		{
			closesPoint.push_back(neigbors[k]);
		}

	}
	return closesPoint;
}

