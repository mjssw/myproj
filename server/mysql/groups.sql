/*
Navicat MySQL Data Transfer

Source Server         : mysqlroot
Source Server Version : 50537
Source Host           : 172.18.5.55:3306
Source Database       : groups

Target Server Type    : MYSQL
Target Server Version : 50537
File Encoding         : 65001

Date: 2014-07-31 17:12:50
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
INSERT INTO `groupid` VALUES ('0000000000000000000000000000000000000000000000000000000000000102');

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
INSERT INTO `groups` VALUES ('1', '斗地主高中群', 'group1.png');
INSERT INTO `groups` VALUES ('2', '麻将大学群', 'group3.png');

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
INSERT INTO `group_1` VALUES ('sjj1', '解析', '#7', '00000000000000000000000000000001');
INSERT INTO `group_1` VALUES ('sjj2', '哈罗oo丕', '#9', '00000000000000000000000000000000');

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
INSERT INTO `group_2` VALUES ('sjj1', '解析', '#7', '00000000000000000000000000000000');
INSERT INTO `group_2` VALUES ('sjj3', '大吉ho', '#8', '00000000000000000000000000000001');

-- ----------------------------
-- Procedure structure for CreateGroup
-- ----------------------------
DROP PROCEDURE IF EXISTS `CreateGroup`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `CreateGroup`(IN `groupid_` varchar(128),IN `groupname_` varchar(64),IN `grouphead_` varchar(64),IN `user_` varchar(64),IN `name_` varchar(64),IN `head_` varchar(64))
BEGIN
	DECLARE tablename_ varchar(128);
	DECLARE result int;
  SET tablename_=CONCAT("group_",groupid_);
	#SELECT tablename_;
	SET result=1;
  INSERT INTO groups(id,name,head) values(groupid_,groupname_,grouphead_);
	SELECT ROW_COUNT() into result;
	IF(result>0) THEN
		SET @STMT :=CONCAT("CREATE TABLE ",tablename_," (user varchar(64),name varchar(64),head varchar(64),ismaster int(32));"); 
		PREPARE STMT FROM @STMT; 
		EXECUTE STMT; 
    IF EXISTS( select 1 from information_schema.tables where table_schema=DATABASE() and table_name=tablename_) THEN
			SET @STMT2 :=CONCAT("INSERT INTO ",tablename_," (user,name,head,ismaster) values('",user_,"','",name_,"','",head_,"',1);");
			PREPARE STMT2 FROM @STMT2;
			EXECUTE STMT2; 
			SELECT ROW_COUNT() into result;
			IF(result>0) THEN
				SET result=1;
			ELSE
				SET result=-3;
			END IF;
		ELSE
			SET result=-2;
		END IF;
	ELSE
		SET result=-1;
	END IF;
	SELECT result;

END
;;
DELIMITER ;

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
		UPDATE groupid set nextid=nid;
	END IF;
	SELECT nid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for IncrementNextGroupId
-- ----------------------------
DROP PROCEDURE IF EXISTS `IncrementNextGroupId`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `IncrementNextGroupId`()
BEGIN
	UPDATE groupid set nextid=nextid+1;
END
;;
DELIMITER ;
