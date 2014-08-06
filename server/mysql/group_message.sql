/*
Navicat MySQL Data Transfer

Source Server         : mysqlroot
Source Server Version : 50537
Source Host           : 172.18.5.55:3306
Source Database       : group_message

Target Server Type    : MYSQL
Target Server Version : 50537
File Encoding         : 65001

Date: 2014-08-06 14:26:23
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Procedure structure for GroupMessage
-- ----------------------------
DROP PROCEDURE IF EXISTS `GroupMessage`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `GroupMessage`(IN `tablename_` varchar(64),IN `user_` varchar(64),IN `msg_` varchar(1024),IN `time_` int(32),IN `check_` int(32))
BEGIN
	DECLARE result int;
	SET result=1;
	IF(check_!=0) THEN
		IF EXISTS( select 1 from information_schema.tables where table_schema=DATABASE() and table_name=tablename_) THEN
			SET result=1;
		ELSE
			SET @STMT :=CONCAT("CREATE TABLE ",tablename_," (Idx int(64) NOT NULL AUTO_INCREMENT, User varchar(64),Message varchar(1024),Time int(32),PRIMARY KEY (`Idx`));"); 
			PREPARE STMT FROM @STMT; 
			EXECUTE STMT;
			IF EXISTS( select 1 from information_schema.tables where table_schema=DATABASE() and table_name=tablename_) THEN
				SET result=1;
			ELSE
				SET result=-1;
			END IF;
		END IF;
	END IF;

	IF(result=1) THEN
		SET @STMT5 :=CONCAT("INSERT INTO ",tablename_,"(User,Message,Time) VALUES('",user_,"','",msg_,"',",time_,");");
		PREPARE STMT5 FROM @STMT5;
		EXECUTE STMT5;
		SELECT ROW_COUNT() into result;
		IF(result>0) THEN
			SET result=1;
		ELSE
			SET result=-2;
		END IF;
	END IF;

	SELECT result;

END
;;
DELIMITER ;
