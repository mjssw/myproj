/*
Navicat MySQL Data Transfer

Source Server         : mysqlroot
Source Server Version : 50537
Source Host           : 172.18.5.55:3306
Source Database       : ud1

Target Server Type    : MYSQL
Target Server Version : 50537
File Encoding         : 65001

Date: 2014-07-30 15:01:44
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `Idx` int(64) NOT NULL AUTO_INCREMENT,
  `User` varchar(64) NOT NULL,
  `Password` varchar(255) NOT NULL,
  `Name` varchar(64) NOT NULL DEFAULT '',
  `Head` varchar(255) DEFAULT '',
  `Sex` int(8) unsigned zerofill DEFAULT '00000000',
  `Exp` bigint(128) unsigned zerofill DEFAULT '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
  `Level` bigint(128) unsigned zerofill DEFAULT '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
  `Gold` bigint(128) unsigned zerofill DEFAULT '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
  `Diamond` bigint(128) unsigned zerofill DEFAULT '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000',
  `Detail` blob,
  `LastLoginTime` int(32) unsigned zerofill DEFAULT NULL,
  `LastLogoutTime` int(32) unsigned zerofill DEFAULT NULL,
  PRIMARY KEY (`Idx`,`User`),
  KEY `UserIndex` (`User`) USING BTREE,
  KEY `IdxIndex` (`Idx`) USING HASH
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO `user` VALUES ('7', 'sjj1', '123456', '解析', '#7', '00000000', '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000360', '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003', '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100', '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010', null, '00000000000000000000001406703536', null);
INSERT INTO `user` VALUES ('8', 'sjj2', '123456', '哈罗oo丕', '#9', '00000000', '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000012', '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000009', '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001234', '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000009', null, '00000000000000000000001406020520', null);
INSERT INTO `user` VALUES ('9', 'sjj3', '123456', '大吉ho', '#8', '00000000', '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000', '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000', '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000', '00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000', null, '00000000000000000000001406703829', null);

-- ----------------------------
-- Table structure for user_sjj1_group
-- ----------------------------
DROP TABLE IF EXISTS `user_sjj1_group`;
CREATE TABLE `user_sjj1_group` (
  `groupid` bigint(64) NOT NULL,
  PRIMARY KEY (`groupid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_sjj1_group
-- ----------------------------
INSERT INTO `user_sjj1_group` VALUES ('1');
INSERT INTO `user_sjj1_group` VALUES ('2');

-- ----------------------------
-- Table structure for user_sjj2_group
-- ----------------------------
DROP TABLE IF EXISTS `user_sjj2_group`;
CREATE TABLE `user_sjj2_group` (
  `groupid` bigint(64) NOT NULL,
  PRIMARY KEY (`groupid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_sjj2_group
-- ----------------------------
INSERT INTO `user_sjj2_group` VALUES ('1');

-- ----------------------------
-- Table structure for user_sjj3_group
-- ----------------------------
DROP TABLE IF EXISTS `user_sjj3_group`;
CREATE TABLE `user_sjj3_group` (
  `groupid` bigint(64) NOT NULL,
  PRIMARY KEY (`groupid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_sjj3_group
-- ----------------------------
INSERT INTO `user_sjj3_group` VALUES ('2');

-- ----------------------------
-- Procedure structure for TestProc
-- ----------------------------
DROP PROCEDURE IF EXISTS `TestProc`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `TestProc`()
BEGIN
	DECLARE num int;
	select count(*) into num from User;
	select num;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for UserLogin
-- ----------------------------
DROP PROCEDURE IF EXISTS `UserLogin`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `UserLogin`(IN `user_` varchar(64),IN `password_` varchar(255))
BEGIN
	DECLARE result,idx_ int;
	set result=0;
	set idx_=-1;
	SELECT Idx into idx_ from user where User=user_ and Password=password_;
	IF(idx_>0) THEN
		SET result=1;
		UPDATE user set LastLoginTime=unix_timestamp() where Idx=idx_;
	END IF;
	SELECT result;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for UserRegister
-- ----------------------------
DROP PROCEDURE IF EXISTS `UserRegister`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `UserRegister`(IN `user_` varchar(64),IN `password_` varchar(255),IN `name_` varchar(64))
BEGIN
	DECLARE num,result int;
	set num=0;
	set result=1;
	SELECT count(*) into num from user where User=user_;
	IF(num>0) THEN
		SET result=0;
	ELSE
		INSERT INTO User(User,Password,Name) values(user_,password_,name_);
	END IF;
	SELECT result;
END
;;
DELIMITER ;
