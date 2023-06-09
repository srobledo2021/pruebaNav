# Copyright 2021 Intelligent Robotics Lab
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node


def generate_launch_description():

    navigate_cmd = Node(
        package='navigate_bt',
        executable='navigate_main',
        parameters=[{
          'use_sim_time': False
        }],
        remappings=[
          ('input_scan', '/scan_raw'),
          ('output_vel', '/nav_vel')
        ],
        output='screen'
    )
    dialogflow_dir = get_package_share_directory('dialogflow_ros2')
    soundplay_node = launch_ros.actions.Node(
        package='sound_play',
        executable='soundplay_node.py',
        output='screen'
    )

    dialogflow_launch = IncludeLaunchDescription(
            PythonLaunchDescriptionSource(os.path.join(
                                            dialogflow_dir, 'launch/',
                                            'dialogflow.launch.py')),)

    ld = LaunchDescription()
    ld.add_action(navigate_cmd)
    ld.add_action(soundplay_node)
    ld.add_action(dialogflow_launch)
    return ld