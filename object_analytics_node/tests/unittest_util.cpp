/*
 * Copyright (c) 2018 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define PCL_NO_PRECOMPILE
#include <gtest/gtest.h>
#include <pcl/io/pcd_io.h>
#include <string>
#include <utility>
#include <vector>

#include "object_analytics_node/model/object_utils.hpp"
#include "unittest_util.hpp"

using geometry_msgs::msg::Point32;
using sensor_msgs::msg::RegionOfInterest;
using object_msgs::msg::Object;
using object_msgs::msg::ObjectInBox;
using object_msgs::msg::ObjectsInBoxes;
using object_analytics_msgs::msg::ObjectInBox3D;
using object_analytics_msgs::msg::ObjectsInBoxes3D;
using object_analytics_node::model::PointT;
using object_analytics_node::model::PointCloudT;
using object_analytics_node::model::Object2D;
using object_analytics_node::model::Object3D;
using object_analytics_node::model::ObjectUtils;

void readPointCloudFromPCD(const std::string & name, PointCloudT::Ptr & pointcloud)
{
  if (pcl::io::loadPCDFile<PointT>(name.c_str(), *pointcloud) == -1) {
    assert(false);
  }
}

Object getObject(const std::string & name, const float probability)
{
  Object obj;
  obj.object_name = name;
  obj.probability = probability;
  return obj;
}

RegionOfInterest getRoi(int x_offset, int y_offset, int width, int height)
{
  RegionOfInterest roi;
  roi.x_offset = x_offset;
  roi.y_offset = y_offset;
  roi.width = width;
  roi.height = height;
  return roi;
}

ObjectInBox getObjectInBox(
  int x_offset, int y_offset, int width, int height,
  const std::string & name, const float probability)
{
  ObjectInBox oib;
  oib.roi = getRoi(x_offset, y_offset, width, height);
  oib.object = getObject(name, probability);
  return oib;
}

Point32 getPoint32(float x, float y, float z)
{
  Point32 point;
  point.x = x;
  point.y = y;
  point.z = z;
  return point;
}

ObjectInBox3D getObjectInBox3D(
  int x_offset, int y_offset, int width, int height, int min_x, int min_y, int min_z,
  int max_x, int max_y, int max_z, const std::string & name, const float probability)
{
  ObjectInBox3D oib3d;
  oib3d.roi = getRoi(x_offset, y_offset, width, height);
  oib3d.max = getPoint32(max_x, max_y, max_z);
  oib3d.min = getPoint32(min_x, min_y, min_z);
  oib3d.object = getObject(name, probability);
  return oib3d;
}

bool operator==(const ObjectInBox & left, const Object2D & right)
{
  if (left.object.object_name == right.getObject().object_name && left.roi == right.getRoi()) {
    return true;
  }

  return false;
}

bool operator==(const Object2D & left, const Object2D & right)
{
  if (left.getRoi() == right.getRoi() &&
    left.getObject().object_name == right.getObject().object_name)
  {
    return true;
  }

  return false;
}

bool operator==(const Object3D & left, const Object3D & right)
{
  if (left.getRoi() == right.getRoi() &&
    left.getMax() == right.getMax() && left.getMin() == right.getMin())
  {
    return true;
  }

  return false;
}

PointT getPointT(float x, float y, float z)
{
  PointT p;
  p.x = x;
  p.y = y;
  p.z = z;
  return p;
}

bool operator==(const PointT & left, const PointT & right)
{
  if (left.x != right.x || left.y != right.y || left.z != right.z) {
    return false;
  }
  return true;
}

bool operator==(const PointXYZPixel & left, const PointT & right)
{
  if (left.x != right.x || left.y != right.y || left.z != right.z) {
    return false;
  }
  return true;
}

bool operator==(const ObjectInBox3D & left, const Object3D & right)
{
  if (left.roi == right.getRoi() && left.max == right.getMax() && left.min == right.getMin()) {
    return true;
  }

  return false;
}

Header createHeader(const builtin_interfaces::msg::Time & stamp, const std::string & frame_id)
{
  Header header;
  header.stamp = stamp;
  header.frame_id = frame_id;
  return header;
}
