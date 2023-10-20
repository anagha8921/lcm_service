#include "rclcpp/rclcpp.hpp"
//#include "tutorial_interfaces/srv/add_three_ints.hpp"     // CHANGE
#include "tutorial_interfaces/msg/service.hpp"
//#include "tutorial_interfaces/msg/num.hpp"
#include "tutorial_interfaces/srv/shutdown.hpp"


#include <memory>

void shutdown(const std::shared_ptr<tutorial_interfaces::srv::Shutdown::Request> request,     // CHANGE
          std::shared_ptr<tutorial_interfaces::srv::Shutdown::Response> response)  // CHANGE
{
   RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Incoming request\na: %ld",   // CHANGE
                request->req);
   auto service_response = std::make_shared<tutorial_interfaces::msg::Service>();
  if(request->req==0)
  {
    service_response->resp=0;
    service_response->service_name="a";     
    response->resp=service_response;                                                         
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Shutdown of node a success!!: [%ld]", (long int)response->resp);
    rclcpp::shutdown();  
  }
  else{
    response->resp=service_response; 
    // response->msg="1a";  
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Shutdown of node a failed!!: [%ld]", (long int)response->resp);
  }
  
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("server_a");  // CHANGE

  rclcpp::Service<tutorial_interfaces::srv::Shutdown>::SharedPtr service =                 // CHANGE
    node->create_service<tutorial_interfaces::srv::Shutdown>("service_a",  &shutdown);     // CHANGE

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to shutdown node a....");      // CHANGE

  rclcpp::spin(node);
  rclcpp::shutdown();
}