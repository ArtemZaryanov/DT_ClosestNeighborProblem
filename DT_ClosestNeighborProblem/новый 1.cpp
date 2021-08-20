RIGHT = "RIGHT"
LEFT = "LEFT"
bool inside_convex_polygon(Point point std::vector<Edge> polygon)
{
	previou_side = nullptr;
	int n_vertices = polygon.size();
	for(int i=0;i<n_vertices-1;i++)
	{
		a = polygon[i]
		b = polygon[(i+1)%n_vertices]
		affine_segment = v_sub(b,a);
		affine_point = v_sub(point,a);
		current_side = get_side(affine_segment,affine_point);
		if(current_side ==nullptr)
		{
			return false;
		}
		else
		{
			if(previou_side ==nullptr
			{
				previou_side = current_side;
			}
			else
			{
				if(previou_side!=current_side)
				{
					return false;
				}
			}
		}
	}
	return true;
}
std::string get_side(int a, int b)
{
	int x = cosine_sign(a,b)
	if(x<0)
	{
		return LEFT;
	}
	else
	{
		if(x>0)
		{
			return RIGHT
		}
		else
		{
			return nullptr
		}
	}
	
}
int v_sub(Edge a,Edge b)
{
	return (a[0]-b[0],a[1]-b[1]);
}
int cosine_sign(Edge a,Edge b)
{
	return a.x0*b.y0-a.y0*b.x0;
}