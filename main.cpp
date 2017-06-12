#include "tetgen.h"

#include <cassert>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
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
    cout << 1 << " " << 2 << " " << (out.trifacelist[i] - 1) << " " << (out.trifacelist[i + 1] - 1) << " " << (out.trifacelist[i + 2] - 1) << endl; 
  }

  cout << endl;

  cout << "vertices" << endl;
  cout << out.numberofpoints << endl;
  cout << 3 << endl;
  for (int i = 0; i < out.numberofpoints; i++) {
    cout << out.pointlist[i] << " " << out.pointlist[i + 1] << " " << out.pointlist[i + 2] << endl;
  }

  return 0;
  
}
