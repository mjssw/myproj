/*
Navicat MySQL Data Transfer

Source Server         : mysqlroot
Source Server Version : 50537
Source Host           : 172.18.5.55:3306
Source Database       : group

Target Server Type    : MYSQL
Target Server Version : 50537
File Encoding         : 65001

Date: 2014-07-23 14:08:44
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for group1
-- ----------------------------
DROP TABLE IF EXISTS `group1`;
CREATE TABLE `group1` (
  `id` bigint(64) NOT NULL,
  `members` blob,
  `creater` varchar(64) NOT NULL DEFAULT '',
  `createtime` int(32) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `IdIndex` (`id`) USING HASH
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of group1
-- ----------------------------
