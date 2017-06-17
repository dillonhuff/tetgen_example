#include "tetgen.h"

#include "parse_stl.h"

#include <cassert>
#include <iostream>

using namespace std;
using namespace stl;

void write_to_poly_file(const stl_data& md) {
  cout << "# Node list" << endl;
  cout << md.triangles.size()*3 << " " << 3 << " " << 0 << " " << 0 << endl;
  int i = 1;
  for (auto& t : md.triangles) {
    cout << i << " " << t.v1.x << " " << t.v1.y << " " << t.v1.z << endl;
    i++;
    cout << i << " " << t.v2.x << " " << t.v2.y << " " << t.v2.z << endl;
    i++;
    cout << i << " " << t.v3.x << " " << t.v3.y << " " << t.v3.z << endl;
    i++;
  }

  cout << endl;

  cout << "# Facet list" << endl;
  cout << md.triangles.size() << " " << 0 << endl;
  int j = 0;
  for (int i = 0; i < md.triangles.size(); i++) {
    cout << 1 << endl;
    cout << 3 << " ";
    cout << j << " ";
    j++;
    cout << j << " ";
    j++;
    cout << j << " ";
    j++;
    cout << endl;
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

  // stl_data mesh_data = parse_stl("./Box1x1x1.stl");
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
