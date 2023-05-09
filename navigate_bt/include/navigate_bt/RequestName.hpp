#ifndef NAVIGATE_BT__REQUESTNAME_HPP_
#define NAVIGATE_BT__REQUESTNAME_HPP_

#include <string>

#include "sound_play.hpp"
#include "behaviortree_cpp_v3/behavior_tree.h"
#include "behaviortree_cpp_v3/bt_factory.h"
#include "gb_dialog/DialogInterface.hpp"

namespace navigate_bt
{

class RequestName : public BT::ActionNodeBase
{
public:
  explicit RequestName(
    const std::string & xml_tag_name,
    const BT::NodeConfiguration & conf);


  void RequestNameCB(dialogflow_ros2_interfaces::msg::DialogflowResult result);

  void halt() {}
  BT::NodeStatus tick();

  static BT::PortsList providedPorts()
  {
    return{
      BT::OutputPort<std::string>("nameID")
      };
  }

private:
  rclcpp::Node::SharedPtr node_;
  gb_dialog::DialogInterface dialog_;
  rclcpp::Time  start_time_;
  std::string name_ = "";
  bool listening_ = false;
};

}  // namespace navigate_bt

#endif  // NAVIGATE_BT__REQUESTNAME_HPP_