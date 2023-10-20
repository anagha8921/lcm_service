#include "rclcpp/rclcpp.hpp"
//#include "tutorial_interfaces/srv/add_three_ints.hpp"        // CHANGE
//#include "tutorial_interfaces/msg/service_response.hpp"
#include "tutorial_interfaces/srv/shutdown.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>

using namespace std::chrono_literals;
std::vector<rclcpp::Client<tutorial_interfaces::srv::Shutdown>::FutureAndRequestId> futures;
void processFuture(rclcpp::Client<tutorial_interfaces::srv::Shutdown>::FutureAndRequestId future) {
    if (future.valid() && future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        auto result = future.get();
        // Process the result
        RCLCPP_INFO(rclcpp::get_logger("async_futures_example"), "Future result: %d", result);
    }
}
class Client : public rclcpp::Node
{
public:
rclcpp::TimerBase::SharedPtr timer_;
size_t count_;
Client()
  : Node("client"), count_(0)
  {
    client =  this->create_client<tutorial_interfaces::srv::Shutdown>("service_a"); 
    client2 = this->create_client<tutorial_interfaces::srv::Shutdown>("service_b");   
    auto request = std::make_shared<tutorial_interfaces::srv::Shutdown::Request>();               // CHANGE
    request->req = 0;
     timer_ = this->create_wall_timer(
       1s, std::bind(&Client::timer_callback, this));
    
  }

 private:
 rclcpp::Client<tutorial_interfaces::srv::Shutdown>::SharedPtr  client;
 rclcpp::Client<tutorial_interfaces::srv::Shutdown>::SharedPtr  client2;
  void timer_callback()
  {
    // rclcpp::Client<tutorial_interfaces::srv::Shutdown>::SharedPtr client =                        // CHANGE
    // this->create_client<tutorial_interfaces::srv::Shutdown>("service_a"); 
    // rclcpp::Client<tutorial_interfaces::srv::Shutdown>::SharedPtr client2 =                        // CHANGE
    // this->create_client<tutorial_interfaces::srv::Shutdown>("service_b");   
    auto request = std::make_shared<tutorial_interfaces::srv::Shutdown::Request>();               // CHANGE
    request->req = 0;// CHANGE
    auto result = client->async_send_request(request);
    auto result2 = client2->async_send_request(request);
    futures.push_back(std::move(result)); 
    futures.push_back(std::move(result2));
    timer_ = this->create_wall_timer(1s, std::bind(&Client::timer_callback, this)); 
     for (size_t i = 0; i < futures.size(); ++i) 
     {
        // Check if the future completed successfully
        if (futures[i].valid() && futures[i].wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            auto response = futures[i].get();
             RCLCPP_INFO(this->get_logger(), "Response %d", response->resp );
            // Handle the response
            RCLCPP_INFO(this->get_logger(), "Service response for request %zu: %s", i, response->resp ? "Failure":"Success"  );
        } else {
            RCLCPP_ERROR(this->get_logger(), "Service request %zu failed", i);
        }
    }
  }
};
int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  //std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("client"); // CHANGE
  auto node = std::make_shared<Client>();
  // rclcpp::Client<tutorial_interfaces::srv::Shutdown>::SharedPtr client =                        // CHANGE
  //   node->create_client<tutorial_interfaces::srv::Shutdown>("service_a"); 
  // rclcpp::Client<tutorial_interfaces::srv::Shutdown>::SharedPtr client2 =                        // CHANGE
  //   node->create_client<tutorial_interfaces::srv::Shutdown>("service_b");                  // CHANGE

  

  // auto request = std::make_shared<tutorial_interfaces::srv::Shutdown::Request>();               // CHANGE
  // request->req = atoll(argv[1]);// CHANGE



  // auto result = client->async_send_request(request);
  
  // Wait for the result.
  // if (rclcpp::spin_until_future_complete(node, result) ==
  //   rclcpp::FutureReturnCode::SUCCESS)
  // {
  //   if( result.get()->resp==0)
  //   {
  //   RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Noda a shutdown SUCCESS!!");
  //   }
  //   else{
  //    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Noda a shutdown FAILED!!"); 
  //   }
   
  // } else {
  //   RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to shutdown Noda a");    // CHANGE
  // }

  // auto result2 = client2->async_send_request(request);
 
  // Wait for the result.
//   if (rclcpp::spin_until_future_complete(node, result2) ==
//     rclcpp::FutureReturnCode::SUCCESS)
//  {
//     if( result2.get()->resp==0)
//     {
//     RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Noda b shutdown SUCCESS!!");
//     }
//     else{
//      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Noda b shutdown FAILED!!"); 
//     }
//   } else {
//     RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to shutdown Noda b");    // CHANGE
//   }

  
  // std::vector<rclcpp::Client<tutorial_interfaces::srv::Shutdown>::FutureAndRequestId> futures;


  // futures.push_back(std::move(result)); 
  // futures.push_back(std::move(result2));

    // rclcpp::WallRate rate(1);  // Adjust the rate as needed

    // while (rclcpp::ok()) {
    //    for (size_t i = 0; i < futures.size(); ++i) {
    //     // Check if the future completed successfully
    //     if (futures[i].valid() && futures[i].wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
    //         auto response = futures[i].get()->resp;
    //        // RCLCPP_INFO(node->get_logger(), "Service response for request %zu: %s", i, response);
    //         // Handle the response
    //         RCLCPP_INFO(node->get_logger(), "Service response for request %zu: %s", i, response ? "Success" : "Failure");
    //     } else {
    //         RCLCPP_ERROR(node->get_logger(), "Service request %zu failed", i);
    //     }
    //    }

    //     rate.sleep();
    // }
  // Wait for all the futures to complete
    rclcpp::spin(node);
  

    // for (size_t i = 0; i < futures.size(); ++i) {
    //     // Check if the future completed successfully
    //     if (futures[i].valid() && futures[i].wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
    //         auto response = futures[i].get();
    //         // Handle the response
    //         RCLCPP_INFO(node->get_logger(), "Service response for request %zu: %s", i, response ? "Success" : "Failure");
    //     } else {
    //         RCLCPP_ERROR(node->get_logger(), "Service request %zu failed", i);
    //     }
    // }
  
    
  rclcpp::shutdown();
  return 0;
}