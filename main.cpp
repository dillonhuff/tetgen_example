#include "tetgen.h"

#include <cassert>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  tetgenbehavior b;

  b.plc = true;
  b.refine = false;
  string infile = "./example";
  for (int i = 0; i < infile.size(); i++) {
    b.infilename[i] = infile[i];
  }
  b.infilename[infile.size()] = '\0';
  
  b.object = tetgenbehavior::POLY;

  strcpy(b.outfilename, b.infilename);
  strcat(b.outfilename, ".1");

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

  cout << "Tetrahedra corners" << endl;
  for (int i = 0; i < out.numberoftetrahedra; i++) {
    cout << out.tetrahedronlist[i] << "\t" << out.tetrahedronlist[i + 1] << "\t" << out.tetrahedronlist[i + 2] << "\t" << out.tetrahedronlist[i + 3] << endl;

    cout << "Points = " << out.pointlist[out.tetrahedronlist[i]] << endl;
    cout << "Points = " << out.pointlist[out.tetrahedronlist[i+1]] << endl;
    cout << "Points = " << out.pointlist[out.tetrahedronlist[i+2]] << endl;
  }

  
  return 0;
  
}
