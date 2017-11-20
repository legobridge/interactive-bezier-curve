#include <fstream>
#include <vector>
#include <bezier1.h>
#include <math.h>
#include <mesh.h>

Mesh surface (vector<glm::vec3> curve, int delta_a = 10)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	Vertex p;
	int angle = 0;

	while (angle < 360)
	{
		for (size_t i = 0; i < curve.size(); i++) {

			vector<glm::vec3> c;
			
			p.Position.x = curve[i].x * (float)cos(glm::radians((float)angle));
			p.Position.y = curve[i].y;
			p.Position.z = curve[i].x * (float)sin(glm::radians((float)angle));

			vertices.push_back(p);
		}
		angle += delta_a;
	}

	for (size_t i = 0; i < vertices.size(); i += curve.size())
	{
		for (size_t j = i; j < i + curve.size() - 1; j++)
		{
			indices.push_back(j % vertices.size());
			indices.push_back((j + curve.size() + 1) % vertices.size());
			indices.push_back((j + curve.size()) % vertices.size());
			indices.push_back(j % vertices.size());
			indices.push_back((j + 1) % vertices.size());
			indices.push_back((j + curve.size() + 1) % vertices.size());
		}
	}

	Mesh mesh(vertices, indices);

	return mesh;
}

bool write_mesh(Mesh mesh)
{
	ofstream file;
	file.open("mesh.off");

	if (file.is_open())
	{
		file << "OFF" << endl << endl;
		file << mesh.vertices.size() << " " << mesh.indices.size() / 3 << endl;

		for (size_t i = 0; i < mesh.vertices.size(); i++)
		{
			file << mesh.vertices[i].Position.x << " " << mesh.vertices[i].Position.y << " " 
				<< mesh.vertices[i].Position.z << endl;
		}

		for (size_t i = 0; i < mesh.indices.size(); i += 3)
		{
			file << 3 << " " << mesh.indices[i] << " " << mesh.indices[i + 1] << " " 
				<< mesh.indices[i + 2] << endl;
		}

		file.close();

		return true;
	}

	return false;
}

int main()
{
	vector<glm::vec3> ps;
	vector<glm::vec3> c;
	int n;

	cin >> n;

	for (int i = 0; i < n; i++)
	{
		glm::vec3 p;
		cin >> p.x >> p.y >> p.z;
		ps.push_back(p);
	}

	cout << endl;

	c = bezier(ps);

	Mesh m = surface(c, 30);

	if (write_mesh(m)) 
	{
		cout << "Surface of revolution about Y axis written to mesh.off." << endl;
	}
	else
	{
		cout << "Write failed." << endl;
	}

	return 0;
}