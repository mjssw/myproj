/*
Navicat MySQL Data Transfer

Source Server         : mysqlroot
Source Server Version : 50537
Source Host           : 172.18.5.55:3306
Source Database       : groups

Target Server Type    : MYSQL
Target Server Version : 50537
File Encoding         : 65001

Date: 2014-09-19 16:41:03
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
-- Table structure for group_102
-- ----------------------------
DROP TABLE IF EXISTS `group_102`;
CREATE TABLE `group_102` (
  `user` varchar(64) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for group_103
-- ----------------------------
DROP TABLE IF EXISTS `group_103`;
CREATE TABLE `group_103` (
  `user` varchar(64) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for group_104
-- ----------------------------
DROP TABLE IF EXISTS `group_104`;
CREATE TABLE `group_104` (
  `user` varchar(64) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for group_105
-- ----------------------------
DROP TABLE IF EXISTS `group_105`;
CREATE TABLE `group_105` (
  `user` varchar(64) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for group_106
-- ----------------------------
DROP TABLE IF EXISTS `group_106`;
CREATE TABLE `group_106` (
  `user` varchar(64) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for group_107
-- ----------------------------
DROP TABLE IF EXISTS `group_107`;
CREATE TABLE `group_107` (
  `user` varchar(64) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for group_108
-- ----------------------------
DROP TABLE IF EXISTS `group_108`;
CREATE TABLE `group_108` (
  `user` varchar(64) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for group_109
-- ----------------------------
DROP TABLE IF EXISTS `group_109`;
CREATE TABLE `group_109` (
  `user` varchar(64) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for group_110
-- ----------------------------
DROP TABLE IF EXISTS `group_110`;
CREATE TABLE `group_110` (
  `user` varchar(64) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for group_111
-- ----------------------------
DROP TABLE IF EXISTS `group_111`;
CREATE TABLE `group_111` (
  `user` varchar(64) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for group_112
-- ----------------------------
DROP TABLE IF EXISTS `group_112`;
CREATE TABLE `group_112` (
  `user` varchar(64) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for group_113
-- ----------------------------
DROP TABLE IF EXISTS `group_113`;
CREATE TABLE `group_113` (
  `user` varchar(64) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for group_114
-- ----------------------------
DROP TABLE IF EXISTS `group_114`;
CREATE TABLE `group_114` (
  `user` varchar(64) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for group_115
-- ----------------------------
DROP TABLE IF EXISTS `group_115`;
CREATE TABLE `group_115` (
  `user` varchar(64) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for group_116
-- ----------------------------
DROP TABLE IF EXISTS `group_116`;
CREATE TABLE `group_116` (
  `user` varchar(64) DEFAULT NULL,
  `name` varchar(64) DEFAULT NULL,
  `head` varchar(64) DEFAULT NULL,
  `ismaster` int(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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
-- Procedure structure for AddMemberToGroup
-- ----------------------------
DROP PROCEDURE IF EXISTS `AddMemberToGroup`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `AddMemberToGroup`(IN `user_` varchar(64),IN `name_` varchar(64),IN `head_` varchar(64),IN `groupid_` bigint(128),IN `dbname_` varchar(64))
BEGIN
	DECLARE result int;
	SET result=1;

	SET @insertsql :=CONCAT("INSERT INTO group_",groupid_," (user,name,head,ismaster) values('",user_,"','",name_,"','",head_,"',0);");
	PREPARE insertsql FROM @insertsql; 
	EXECUTE insertsql; 

	SELECT ROW_COUNT() into result;
	IF(result>0) THEN
		SET @STMT5 :=CONCAT("INSERT INTO ",dbname_,".user_",user_,"_group(groupid) values(",groupid_,");");
		PREPARE STMT5 FROM @STMT5;
		EXECUTE STMT5;

		SELECT ROW_COUNT() into result;
		IF(result>0) THEN
			SET result=1;
		ELSE
			SET @delsql :=CONCAT("DELETE FROM group_",groupid_," WHERE user='",user_,"';");
			PREPARE delsql FROM @delsql; 
			EXECUTE delsql; 
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
-- Procedure structure for AskJoinGroup
-- ----------------------------
DROP PROCEDURE IF EXISTS `AskJoinGroup`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `AskJoinGroup`(IN `groupid_` bigint(128),IN `user_` varchar(64))
BEGIN
  DECLARE tablename_ varchar(128);
	DECLARE result int;

	# (1) find group is EXISTS
	SET result=0;
	SELECT COUNT(*) into result from groups where id=groupid_;
	IF(result=0) THEN
		SET result=-4;
	ELSE
		SET tablename_=CONCAT("join_group_",groupid_);
		IF NOT EXISTS( select 1 from information_schema.tables where table_schema=DATABASE() and table_name=tablename_) THEN
			SET @STMT :=CONCAT("CREATE TABLE ",tablename_," (user varchar(64),state int(32));"); 
			PREPARE STMT FROM @STMT; 
			EXECUTE STMT; 
		END IF;
		SET result=-1;
		# state: 
		# 	0: valid request, more same request is ignored
		#		1: request processed, aleardy in groupid, more request is ignored
		# 	2: request processed, rejust, can apply join again
		#		3: request processed, rejust, can not apply join any more, feedback user
		IF EXISTS( select 1 from information_schema.tables where table_schema=DATABASE() and table_name=tablename_) THEN
			SET result=1;
			SET @joinstate=-1;
			SET @STMT :=CONCAT("SELECT state into @joinstate FROM ",tablename_," where user='", user_, "';");
			PREPARE STMT FROM @STMT; 
			EXECUTE STMT;
			SELECT @joinstate;
			IF(@joinstate!=-1) THEN
				IF(@joinstate=1) THEN
					SET result=-2;
				ELSEIF(@joinstate=2) THEN
					SET @STMT :=CONCAT("UPDATE ",tablename_," SET state=0 where user='", user_, "';"); 
					PREPARE STMT FROM @STMT; 
					EXECUTE STMT;
					# TODO check
				ELSEIF(@joinstate=3) THEN
					SET result=-3;
				ELSE
					SET result=1;
				END IF;
			ELSE
				SET @STMT :=CONCAT("INSERT INTO ",tablename_,"(user,state) VALUES('", user_, "',0);"); 
				PREPARE STMT FROM @STMT; 
				EXECUTE STMT;
				# TODO check insert result
			END IF;
		END IF;
	END IF;
	SELECT result;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for CreateGroup
-- ----------------------------
DROP PROCEDURE IF EXISTS `CreateGroup`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `CreateGroup`(IN `groupid_` varchar(128),IN `groupname_` varchar(64),IN `grouphead_` varchar(64),IN `user_` varchar(64),IN `name_` varchar(64),IN `head_` varchar(64),IN `udname_` varchar(64))
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
				SET @STMT5 :=CONCAT("INSERT INTO ",udname_,".user_",user_,"_group(groupid) values(",groupid_,");");
				PREPARE STMT5 FROM @STMT5;
				EXECUTE STMT5;
				SELECT ROW_COUNT() into result;
				IF(result>0) THEN
					SET result=1;
				ELSE
					DELETE FROM groups where id=groupid_;
					SET @STMT6 :=CONCAT("DROP TABLE ",tablename_,";");
					PREPARE STMT6 FROM @STMT6;
					EXECUTE STMT6;
					SET result=-4;
				END IF;
			ELSE
				DELETE FROM groups where id=groupid_;
				SET @STMT3 :=CONCAT("DROP TABLE ",tablename_,";");
				PREPARE STMT3 FROM @STMT3;
				EXECUTE STMT3;
				SET result=-3;
			END IF;
		ELSE
			DELETE FROM groups where id=groupid_;
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

-- ----------------------------
-- Procedure structure for MemberLeaveGroup
-- ----------------------------
DROP PROCEDURE IF EXISTS `MemberLeaveGroup`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `MemberLeaveGroup`(IN `user_` varchar(64),IN `group_` bigint(128),IN `udname_` varchar(64))
BEGIN
	DECLARE result int;
	SET result=1;

	SET @delsql :=CONCAT("DELETE FROM group_",group_," WHERE user='",user_,"';");
	#SELECT @delsql;
	PREPARE delsql FROM @delsql; 
	EXECUTE delsql;

	SET @delsql2 :=CONCAT("DELETE FROM ",udname_,".user_",user_,"_group WHERE groupid=",group_,";");
	#SELECT @delsql2;
	PREPARE delsql2 FROM @delsql2; 
	EXECUTE delsql2;

	SELECT result;

END
;;
DELIMITER ;
