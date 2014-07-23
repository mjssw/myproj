/*
Navicat MySQL Data Transfer

Source Server         : mysqlroot
Source Server Version : 50537
Source Host           : 172.18.5.55:3306
Source Database       : group

Target Server Type    : MYSQL
Target Server Version : 50537
File Encoding         : 65001

Date: 2014-07-23 18:05:51
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for groupid
-- ----------------------------
DROP TABLE IF EXISTS `groupid`;
CREATE TABLE `groupid` (
  `nextid` bigint(64) unsigned zerofill NOT NULL DEFAULT '0000000000000000000000000000000000000000000000000000000000000000',
  PRIMARY KEY (`nextid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of groupid
-- ----------------------------
INSERT INTO `groupid` VALUES ('0000000000000000000000000000000000000000000000000000000000000101');

-- ----------------------------
-- Table structure for groups
-- ----------------------------
DROP TABLE IF EXISTS `groups`;
CREATE TABLE `groups` (
  `id` bigint(64) NOT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `idIndex` (`id`) USING HASH
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of groups
-- ----------------------------
INSERT INTO `groups` VALUES ('1', 'gp1', 'group1.png');
INSERT INTO `groups` VALUES ('2', 'gp2', 'group3.png');

-- ----------------------------
-- Table structure for group_1
-- ----------------------------
DROP TABLE IF EXISTS `group_1`;
CREATE TABLE `group_1` (
  `user` varchar(64) NOT NULL DEFAULT '',
  `name` varchar(64) NOT NULL DEFAULT '',
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) unsigned zerofill NOT NULL DEFAULT '00000000000000000000000000000000',
  PRIMARY KEY (`user`),
  KEY `UserIndex` (`user`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of group_1
-- ----------------------------
INSERT INTO `group_1` VALUES ('sjj1', 'abc1', '#7', '00000000000000000000000000000001');
INSERT INTO `group_1` VALUES ('sjj2', 'abc2', '#9', '00000000000000000000000000000000');

-- ----------------------------
-- Table structure for group_2
-- ----------------------------
DROP TABLE IF EXISTS `group_2`;
CREATE TABLE `group_2` (
  `user` varchar(64) NOT NULL DEFAULT '',
  `name` varchar(64) NOT NULL DEFAULT '',
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) unsigned zerofill NOT NULL DEFAULT '00000000000000000000000000000000',
  PRIMARY KEY (`user`),
  KEY `UserIndex` (`user`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of group_2
-- ----------------------------
INSERT INTO `group_2` VALUES ('sjj1', 'abc1', '#7', '00000000000000000000000000000000');
INSERT INTO `group_2` VALUES ('sjj3', 'abc3', '#8', '00000000000000000000000000000001');

-- ----------------------------
-- Procedure structure for GetNextGroupId
-- ----------------------------
DROP PROCEDURE IF EXISTS `GetNextGroupId`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `GetNextGroupId`()
BEGIN
	DECLARE nid int;
	SELECT nextid into nid from groupid;
	IF(nid=0) THEN
		SET nid=1;
	END IF;
	UPDATE groupid set nextid=nid+1;
	SELECT nid;
END
;;
DELIMITER ;
