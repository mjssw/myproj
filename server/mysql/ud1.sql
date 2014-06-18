/*
Navicat MySQL Data Transfer

Source Server         : mysqlroot
Source Server Version : 50537
Source Host           : 172.18.5.55:3306
Source Database       : ud1

Target Server Type    : MYSQL
Target Server Version : 50537
File Encoding         : 65001

Date: 2014-06-18 17:26:10
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
  `Detail` blob,
  `Group` blob,
  `LastLoginTime` int(32) unsigned zerofill DEFAULT NULL,
  `LastLogoutTime` int(32) unsigned zerofill DEFAULT NULL,
  PRIMARY KEY (`Idx`,`User`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

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
