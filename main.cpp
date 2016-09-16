// Reference:
// http://doc.cgal.org/latest/Polygon_mesh_processing/
// section 2.2.3

#include <fstream>
#include <vector>

#include <boost/function_output_iterator.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/graph_traits_Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/border.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Surface_mesh<K::Point_3> Mesh;
typedef boost::graph_traits<Mesh>::halfedge_descriptor halfedge_descriptor;
typedef boost::graph_traits<Mesh>::edge_descriptor     edge_descriptor;

namespace PMP = CGAL::Polygon_mesh_processing;

struct halfedge2edge
{
  halfedge2edge(const Mesh& m, std::vector<edge_descriptor>& edges)
    : m_mesh(m), m_edges(edges)
  {}
  void operator()(const halfedge_descriptor& h) const
  {
    m_edges.push_back(edge(h, m_mesh));
  }
  const Mesh& m_mesh;
  std::vector<edge_descriptor>& m_edges;
};

int main(int argc, char* argv[])
{

  std::string appName = std::string(argv[0]);
  namespace po = boost::program_options; 
  po::options_description desc("Options"); 
  desc.add_options() 
    ("help,h", "Print help messages") 
    ("input,i", po::value<std::string>()->required(), "input mesh.") 
    ("edge_length,e", po::value<double>()->default_value(0.005), "target edge length.")
    ("iteration,r", po::value<int>()->default_value(3), "number of iterations."); 


  po::variables_map vm;  
  po::store(po::parse_command_line(argc, argv, desc), vm);

  if (vm.count("help")) {
    
    std::cout << "Usage: " << std::endl;
    std::cout <<  appName << " -i input_file [-e target_edge_length=0.005] [-r num_iteration] [-h]" << std::endl;

    return 0;
  } 

  const std::string& filename = vm["input"].as<std::string>();
  std::ifstream input(filename);
  Mesh mesh;
  
  if (!input || !(input >> mesh)) {
    std::cerr << "Not a valid off file." << std::endl;
    return 1;
  }

  std::cout << "Original mesh " << filename
    << " (" << num_faces(mesh) << " faces)..." << std::endl;

  const double target_edge_length = vm["edge_length"].as<double>();
  const int nb_iter = vm["iteration"].as<int>();

  std::cout << "Split border...";
  
  std::vector<edge_descriptor> border;
  PMP::border_halfedges(faces(mesh),
    mesh,
    boost::make_function_output_iterator(halfedge2edge(mesh, border)));
  
  PMP::split_long_edges(border, target_edge_length, mesh);

  std::cout << "done." << std::endl;

  std::cout << "Starting remeshing... " << std::endl;
  
  PMP::isotropic_remeshing(
      faces(mesh),
      target_edge_length,
      mesh,
      PMP::parameters::number_of_iterations(nb_iter)
      .protect_constraints(true)//i.e. protect border, here
      );

  std::cout << "Remeshing done." << " (" << num_faces(mesh) << " faces)..." << std::endl;

  boost::filesystem::path p(filename);

  const std::string output_filename = (p.parent_path() /= p.stem()).string() + "_remeshed.off";
  std::ofstream output(output_filename);
  output << mesh;  
  std::cout << "Output to " << output_filename << std::endl;

  return 0;
}
