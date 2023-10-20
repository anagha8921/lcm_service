#include "rclcpp/rclcpp.hpp"
//#include "tutorial_interfaces/srv/add_three_ints.hpp"     // CHANGE
#include "tutorial_interfaces/srv/shutdown.hpp"
//#include "tutorial_interfaces/msg/service_response.hpp"

#include <memory>

void shutdown(const std::shared_ptr<tutorial_interfaces::srv::Shutdown::Request> request,     // CHANGE
          std::shared_ptr<tutorial_interfaces::srv::Shutdown::Response> response)  // CHANGE
{
   RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Incoming request\na: %ld",   // CHANGE
                request->req);
  
  if(request->req==0)
  {
   response->resp=0;                                                                  
   RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Shutdown of node b success!!: [%ld]", (long int)response->resp);
   rclcpp::shutdown();    
  }
  else{
    response->resp=1;     
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Shutdown of node b failure!!: [%ld]", (long int)response->resp);
   
  }
  
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("server_b");  // CHANGE

  rclcpp::Service<tutorial_interfaces::srv::Shutdown>::SharedPtr service =                 // CHANGE
    node->create_service<tutorial_interfaces::srv::Shutdown>("service_b",  &shutdown);     // CHANGE

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to shutdown node b....");      // CHANGE

  rclcpp::spin(node);
  rclcpp::shutdown();
}