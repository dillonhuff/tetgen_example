#include "tetgen.h"

#include "parse_stl.h"

#include <cmath>
#include <cassert>
#include <iostream>

using namespace std;
using namespace stl;

bool within_eps(const point& l, const point& r, double eps) {
  double xd = l.x - r.x;
  double yd = l.y - r.y;
  double zd = l.z - r.z;
  double diff = sqrt(xd*xd + yd*yd + zd*zd);
  return diff <= eps;
}

bool within_eps(double l, double r, double eps) {
  double diff = abs(l - r);
  return diff <= eps;
}

typedef long index_t;

struct triangle_t {
  index_t v[3];

  index_t& i() { return v[0]; }
  const index_t& i() const { return v[0]; }
        
  index_t& j() { return v[1]; }
  const index_t& j() const { return v[1]; }
        
  index_t& k() { return v[2]; }
  const index_t& k() const { return v[2]; }
        
  triangle_t() {
    v[0] = v[1] = v[2] = -1;
  }
};

index_t find_index(point p, std::vector<point>& vertices, double tolerance) {
  for (unsigned i = 0; i < vertices.size(); i++) {
    if (within_eps(p, vertices[i], tolerance)) { return i; }
  }
  vertices.push_back(p);
  return vertices.size() - 1;
}

std::vector<triangle_t>
fill_vertex_triangles_no_winding_check(const std::vector<triangle>& triangles,
				       std::vector<point>& vertices,
				       double tolerance) {
  std::vector<triangle_t> vertex_triangles;
  for (auto t : triangles) {
    auto v1i = find_index(t.v1, vertices, tolerance);
    auto v2i = find_index(t.v2, vertices, tolerance);
    auto v3i = find_index(t.v3, vertices, tolerance);
    triangle_t tr;
    tr.v[0] = v1i;
    tr.v[1] = v2i;
    tr.v[2] = v3i;
    vertex_triangles.push_back(tr);
  }

  return vertex_triangles;
}

void write_to_poly_file(const stl_data& md) {
  vector<point> vertices;
  vector<triangle_t> vertex_triangles =
    fill_vertex_triangles_no_winding_check(md.triangles, vertices, 0.0001);

  cout << "# Node list" << endl;
  cout << vertices.size() << " " << 3 << " " << 0 << " " << 0 << endl;
  int i = 1;
  for (auto& t : vertices) {
    cout << i << " " << t.x << " " << t.y << " " << t.z << endl;
    i++;
    // cout << i << " " << t.v2.x << " " << t.v2.y << " " << t.v2.z << endl;
    // i++;
    // cout << i << " " << t.v3.x << " " << t.v3.y << " " << t.v3.z << endl;
    // i++;
  }

  cout << endl;

  cout << "# Facet list" << endl;
  cout << md.triangles.size() << " " << 0 << endl;
  for (int i = 0; i < vertex_triangles.size(); i++) {
    auto t = vertex_triangles[i];
    cout << 1 << endl;
    cout << 3 << " ";
    cout << (t.v[0] + 1) << " " << (t.v[1] + 1) << " " << (t.v[2] + 1) << endl;
    // cout << j << " ";
    // j++;
    // cout << j << " ";
    // j++;
    // cout << j << " ";
    // j++;
    // cout << endl;
  }

  cout << endl;

  cout << "# Hole list" << endl;
  cout << 0 << endl;
  cout << endl;
  cout << "# Region list" << endl;
  cout << 0 << endl;
  cout << endl;
}

int main(int argc, char *argv[]) {

  // string target_file(argv[1]);
  // stl_data mesh_data = parse_stl(target_file);
  // write_to_poly_file(mesh_data);

  // return 0;
  
  tetgenbehavior b;

  b.parse_commandline(argc, argv);
  // b.plc = true;
  // b.refine = false;
  // string infile = "./example";
  // for (int i = 0; i < infile.size(); i++) {
  //   b.infilename[i] = infile[i];
  // }
  // b.infilename[infile.size()] = '\0';
  
  // b.object = tetgenbehavior::POLY;

  // strcpy(b.outfilename, b.infilename);
  // strcat(b.outfilename, ".1");

  tetgenio in, addin, bgmin;
  
  assert(!b.refine);
  assert(b.plc);
  if (!in.load_plc(b.infilename, (int) b.object)) {
    terminatetetgen(NULL, 10);
  }

  tetgenio out;

  tetrahedralize(&b, &in, &out, &addin, &bgmin);
  //tetrahedralize(&b, &in, NULL, &addin, &bgmin);


  cout << "---------------- MY OUTPUT ----------------" << endl;
  cout << "# of points                 = " << out.numberofpoints << endl;
  cout << "# of tetrahedra             = " << out.numberoftetrahedra << endl;
  cout << "# of corners                = " << out.numberofcorners << endl;
  cout << "# of tetrahedron attributes = " << out.numberoftetrahedronattributes << endl;
  cout << "# of facets                 = " << out.numberoffacets << endl;

  // cout << "Tetrahedra corners" << endl;
  // for (int i = 0; i < out.numberoftetrahedra; i++) {
  //   cout << out.tetrahedronlist[i] << "\t" << out.tetrahedronlist[i + 1] << "\t" << out.tetrahedronlist[i + 2] << "\t" << out.tetrahedronlist[i + 3] << endl;

  //   cout << "Points = " << out.pointlist[out.tetrahedronlist[i]] << endl;
  //   cout << "Points = " << out.pointlist[out.tetrahedronlist[i+1]] << endl;
  //   cout << "Points = " << out.pointlist[out.tetrahedronlist[i+2]] << endl;
  // }

  cout << "MFEM mesh v1.0" << endl << endl;
  cout << "dimension" << endl;
  cout << 3 << endl << endl;

  cout << "elements" << endl;
  cout << out.numberoftetrahedra << endl;

  for (int i = 0; i < out.numberoftetrahedra; i++) {
    int tet_index = 4*i;
    cout << 1 << " " << 4 << " " << (out.tetrahedronlist[tet_index] - 1) << " " << (out.tetrahedronlist[tet_index + 1] - 1) << " " << (out.tetrahedronlist[tet_index + 2] - 1) << " " << (out.tetrahedronlist[tet_index + 3] - 1) << endl;
  }

  cout << endl;

  cout << "boundary" << endl;
  cout << out.numberoftrifaces << endl;
  for (int i = 0; i < out.numberoftrifaces; i++) {
    int tri_index = 3*i;
    cout << 1 << " " << 2 << " " << (out.trifacelist[tri_index] - 1) << " " << (out.trifacelist[tri_index + 1] - 1) << " " << (out.trifacelist[tri_index + 2] - 1) << endl; 
  }

  cout << endl;

  cout << "vertices" << endl;
  cout << out.numberofpoints << endl;
  cout << 3 << endl;
  for (int i = 0; i < out.numberofpoints; i++) {
    int vert_index = 3*i;
    cout << out.pointlist[vert_index] << " " << out.pointlist[vert_index + 1] << " " << out.pointlist[vert_index + 2] << endl;
  }

  return 0;
  
}
