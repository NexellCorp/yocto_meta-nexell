#!/bin/sh -e

# CGI output must start with at least empty line (or headers)
printf "Content-type: text/html\r\n\r\n"

cat <<-EOH
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="Content-Script-Type" content="text/javascript" />
<meta http-equiv="cache-control" content="no-cache" />
<link rel="stylesheet" type="text/css" media="screen" href="/css/cascade.css" />
<!--[if IE 6]><link rel="stylesheet" type="text/css" media="screen" href="/css/ie6.css" /><![endif]-->
<!--[if IE 7]><link rel="stylesheet" type="text/css" media="screen" href="/css/ie7.css" /><![endif]-->
<!--[if IE 8]><link rel="stylesheet" type="text/css" media="screen" href="/css/ie8.css" /><![endif]-->
<script type="text/javascript" src="/js/xhr.js"></script>
<script type="text/javascript" src="/js/jquery-1.10.2.js"></script>
<script type="text/javascript" src="/js/json2.min.js"></script>
<title>Nexell BitMiner</title>
</head>
<body class="lang_en">
	<p class="skiplink">
		<span id="skiplink1"><a href="#navigation">Skip to navigation</a></span>
		<span id="skiplink2"><a href="#content">Skip to content</a></span>
	</p>
	<div id="menubar">
		<h2 class="navigation"><a id="navigation" name="navigation">Navigation</a></h2>
		<div class="clear"></div>
	</div>
	<div id="menubar" style="background-color: #0a2b40;">
		<div class="hostinfo" style="float: left; with: 500px;">
			<img src="/images/Nexell-logo-White_Transpraency_50-1.png" width="178" height="18" alt="" title="" border="0" />
		</div>
		<div class="clear"></div>
	</div>
	<div id="maincontainer">
		<div id="tabmenu">
			<div class="tabmenu1">
				<ul class="tabmenu l1">
					<li class="tabmenu-item-status"><a href="/index.html">System</a></li>
					<li class="tabmenu-item-system"><a href="/cgi-bin/minerConfiguration.cgi">Miner Configuration</a></li>
					<li class="tabmenu-item-network active"><a href="/cgi-bin/minerStatus.cgi">Miner Status</a></li>
					<li class="tabmenu-item-system"><a href="/network.html">Network</a></li>
				</ul>
				<br style="clear: both" />
			</div>
		</div>
		<div id="maincontent">
			<noscript>
				<div class="errorbox">
					<strong>Java Script required!</strong><br /> You must enable Java Script in your browser or LuCI will not work properly.
				</div>
			</noscript>
			<h2 style="padding-bottom:10px;"><a id="content" name="content">Miner Status</a></h2>
			<div class="cbi-map" id="cbi-cgminerstatus">
				<!-- tblsection -->
				<fieldset class="cbi-section" id="cbi-table-table">
					<legend>Summary</legend>
					<div class="cbi-section-descr"></div>
					<div class="cbi-section-node">
						<table class="cbi-section-table">
							<tr class="cbi-section-table-titles">
								<th class="cbi-section-table-cell">Elapsed</th>
								<th class="cbi-section-table-cell">GH/S(5s)</th>
								<th class="cbi-section-table-cell">GH/S(avg)</th>
								<th class="cbi-section-table-cell">FoundBlocks</th>
<th class="cbi-section-table-cell">LocalWork</th>								
<th class="cbi-section-table-cell">Utility</th>
								
								<th class="cbi-section-table-cell">WU</th>
								<th class="cbi-section-table-cell">BestShare</th>
							</tr>
							<tr class="cbi-section-table-descr">
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
							</tr>
EOH

# Read miner status
nexell_elapsed=
nexell_ghs5s=
nexell_ghsav=
nexell_foundblocks=
nexell_getworks=
nexell_accepted=
nexell_rejected=
nexell_hw=
nexell_utility=
nexell_discarded=
nexell_stale=
nexell_localwork=
nexell_wu=
nexell_diffa=
nexell_diffr=
nexell_bestshare=

nexell_tmp=`cgminer-api -o`
if [ "${nexell_tmp}" == "Socket connect failed: Connection refused" ]; then
	nexell_elapsed=0
	nexell_ghs5s=0
	nexell_ghsav=0
	nexell_foundblocks=0
	nexell_getworks=0
	nexell_accepted=0
	nexell_rejected=0
	nexell_hw=0
	nexell_utility=0
	nexell_discarded=0
	nexell_stale=0
	nexell_localwork=0
	nexell_wu=0
	nexell_diffa=0
	nexell_diffr=0
	nexell_bestshare=0
else
	nexell_elapsed=${nexell_tmp#*Elapsed=}
	nexell_elapsed=${nexell_elapsed%%,MHS av=*}
	
	nexell_ghsav=${nexell_tmp#*MHS av=}
	nexell_ghsav=${nexell_ghsav%%,MHS 5s=*}
	
	nexell_ghs5s=${nexell_tmp#*MHS 5s=}
	nexell_ghs5s=${nexell_ghs5s%%,MHS 1m=*}
	
	nexell_foundblocks=${nexell_tmp#*Found Blocks=}
	nexell_foundblocks=${nexell_foundblocks%%,Getworks=*}
	
	nexell_getworks=${nexell_tmp#*Getworks=}
	nexell_getworks=${nexell_getworks%%,Accepted=*}
	
	nexell_accepted=${nexell_tmp#*Accepted=}
	nexell_accepted=${nexell_accepted%%,Rejected=*}
	
	nexell_rejected=${nexell_tmp#*Rejected=}
	nexell_rejected=${nexell_rejected%%,Hardware Errors=*}
	
	nexell_hw=${nexell_tmp#*Hardware Errors=}
	nexell_hw=${nexell_hw%%,Utility=*}
	
	nexell_utility=${nexell_tmp#*Utility=}
	nexell_utility=${nexell_utility%%,Discarded=*}
	
	nexell_discarded=${nexell_tmp#*Discarded=}
	nexell_discarded=${nexell_discarded%%,Stale=*}
	
	nexell_stale=${nexell_tmp#*Stale=}
	nexell_stale=${nexell_stale%%,Get Failures=*}
	
	nexell_localwork=${nexell_tmp#*Local Work=}
	nexell_localwork=${nexell_localwork%%,Remote Failures=*}
	
	nexell_wu=${nexell_tmp#*Work Utility=}
	nexell_wu=${nexell_wu%%,Difficulty Accepted=*}
	
	nexell_diffa=${nexell_tmp#*Difficulty Accepted=}
	nexell_diffa=${nexell_diffa%%,Difficulty Rejected=*}
	nexell_diffa=${nexell_diffa%%.*}
	
	nexell_diffr=${nexell_tmp#*Difficulty Rejected=}
	nexell_diffr=${nexell_diffr%%,Difficulty Stale=*}
	nexell_diffr=${nexell_diffr%%.*}
	
	nexell_diffs=${nexell_tmp#*Difficulty Stale=}
	nexell_diffs=${nexell_diffs%%,Best Share=*}
	nexell_diffs=${nexell_diffs%%.*}
	
	nexell_bestshare=${nexell_tmp#*Best Share=}
	nexell_bestshare=${nexell_bestshare%%,Device Hardware*}
fi

if [ "${nexell_elapsed}" = "" ]; then 
	nexell_elapsed=0
fi

nexell_days=$((${nexell_elapsed}/86400))
nexell_hours=$((${nexell_elapsed}/3600-${nexell_days}*24))
nexell_minutes=$((${nexell_elapsed}/60-${nexell_days}*1440-${nexell_hours}*60))
nexell_seconds=$((${nexell_elapsed}-${nexell_days}*86400-${nexell_hours}*3600-${nexell_minutes}*60))

nexell_elapsed=
if [ ${nexell_days} -gt 0 ]; then
nexell_elapsed=${nexell_elapsed}${nexell_days}d
fi
if [ ${nexell_hours} -gt 0 ]; then
nexell_elapsed=${nexell_elapsed}${nexell_hours}h
fi
if [ ${nexell_minutes} -gt 0 ]; then
nexell_elapsed=${nexell_elapsed}${nexell_minutes}m
fi
if [ ${nexell_seconds} -gt 0 ]; then
nexell_elapsed=${nexell_elapsed}${nexell_seconds}s
fi



echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"nexell_elapsed\">${nexell_elapsed}</div>"
echo "<div id=\"cbip-table-1-elapsed\"></div>"
echo "</td>"
#2014-10-16############################################################################# 
nexell_ghs5s_left=${nexell_ghs5s%%.*}
nexell_ghs5s_left=$(echo $nexell_ghs5s_left| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
nexell_ghs5s_right=${nexell_ghs5s#*.}
nexell_ghs5s_split=${nexell_ghs5s_left}"."${nexell_ghs5s_right}
#######################################################################################
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"nexell_ghs5s\">${nexell_ghs5s_split}</div>"
echo "<div id=\"cbip-table-1-ghs5s\"></div>"
echo "</td>"

#2014-10-16############################################################################# 
nexell_ghsav_left=${nexell_ghsav%%.*}
nexell_ghsav_left=$(echo $nexell_ghsav_left| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
nexell_ghsav_right=${nexell_ghsav#*.}
nexell_ghsav_split=${nexell_ghsav_left}"."${nexell_ghsav_right}
#######################################################################################
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"nexell_ghsav\">${nexell_ghsav_split}</div>"
echo "<div id=\"cbip-table-1-ghsav\"></div>"
echo "</td>"
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"nexell_foundblocks\">${nexell_foundblocks}</div>"
echo "<div id=\"cbip-table-1-foundblocks\"></div>"
echo "</td>"
#2014-10-16############################################################################# 
nexell_localwork_split=$(echo $nexell_localwork| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
#######################################################################################
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"nexell_localwork\">${nexell_localwork_split}</div>"
echo "<div id=\"cbip-table-1-localwork\"></div>"
echo "</td>"
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"nexell_utility\">${nexell_utility}</div>"
echo "<div id=\"cbip-table-1-utility\"></div>"
echo "</td>"

#2014-10-16############################################################################# 
nexell_wu_left=${nexell_wu%%.*}
nexell_wu_left=$(echo $nexell_wu_left| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
nexell_wu_right=${nexell_wu#*.}
nexell_wu_split=${nexell_wu_left}"."${nexell_wu_right}
#######################################################################################
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"nexell_wu\">${nexell_wu_split}</div>"
echo "<div id=\"cbip-table-1-wu\"></div>"
echo "</td>"
echo "<td class=\"cbi-value-field\">"
echo "<div id=\"nexell_bestshare\">${nexell_bestshare}</div>"
echo "<div id=\"cbip-table-1-bestshare\"></div>"
echo "</td>"
echo "</tr>"

cat <<-EOP
						</table>
					</div>
				</fieldset>
				<!-- /tblsection -->
				<!-- tblsection -->
				<fieldset class="cbi-section" id="cbi-table-table">
					<legend>Pools</legend>
					<div class="cbi-section-descr"></div>
					<div class="cbi-section-node">
						<table id="nexell_pools" class="cbi-section-table">
							<tr class="cbi-section-table-titles">
								<th class="cbi-section-table-cell">Pool</th>
								<th class="cbi-section-table-cell">URL</th>
								<th class="cbi-section-table-cell">User</th>
								<th class="cbi-section-table-cell">Status</th>
                                                                 <th class="cbi-section-table-cell">Diff</th>
                                                                 <th class="cbi-section-table-cell">GetWorks</th>
                                                                 <th class="cbi-section-table-cell">Priority</th>
								 <th class="cbi-section-table-cell">Accepted</th>		
                                                                 <th class="cbi-section-table-cell">Diff1#</th>
							 	 <th class="cbi-section-table-cell">DiffA#</th>
								 <th class="cbi-section-table-cell">DiffR#</th>
								 <th class="cbi-section-table-cell">DiffS#</th>						         	
								<th class="cbi-section-table-cell">Rejected</th>
								<th class="cbi-section-table-cell">Discarded</th>
								<th class="cbi-section-table-cell">Stale</th>
								<th class="cbi-section-table-cell">LSDiff</th>
								<th class="cbi-section-table-cell">LSTime</th>
							</tr>
							<tr class="cbi-section-table-descr">
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
							</tr>
EOP

nexell_tmp=`cgminer-api -o pools`
if [ "${nexell_tmp}" != "Socket connect failed: Connection refused" ]; then
	nexell_last_len=0
	nexell_len=0
	nexell_pool_getworks_total=0
	nexell_pool_accepted_total=0
	nexell_pool_rejected_total=0
	nexell_pool_discarded_total=0
	nexell_pool_stale_total=0
	nexell_pool_diff1_total=0
	nexell_pool_diffa_total=0
	nexell_pool_diffr_total=0
	nexell_pool_diffs_total=0
	while :;
	do
		nexell_tmp=${nexell_tmp#*POOL=}
		nexell_len=${#nexell_tmp}
	
		if [ ${nexell_len} -eq ${nexell_last_len} ]; then
			break
		fi
		nexell_last_len=${nexell_len}
		
		echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
		
		nexell_pool_index=
		nexell_pool_url=
		nexell_pool_user=
		nexell_pool_status=
		nexell_pool_priority=
		nexell_pool_getworks=
		nexell_pool_accepted=
		nexell_pool_rejected=
		nexell_pool_discarded=
		nexell_pool_stale=
		nexell_pool_diff=
		nexell_pool_diff1=
		nexell_pool_diffa=
		nexell_pool_diffr=
		nexell_pool_diffs=
		nexell_pool_lsdiff=
		nexell_pool_lstime=
		
		nexell_pool_index=${nexell_tmp%%,URL=*}
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-pool\">${nexell_pool_index}</div>"
		echo "<div id=\"cbip-table-1-pool\"></div>"
		echo "</td>"
		
		nexell_pool_url=${nexell_tmp#*URL=}
		nexell_pool_url=${nexell_pool_url%%,Status=*}
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-url\">${nexell_pool_url}</div>"
		echo "<div id=\"cbip-table-1-url\"></div>"
		echo "</td>"
		
		nexell_pool_user=${nexell_tmp#*User=}
		nexell_pool_user=${nexell_pool_user%%,Last Share Time=*}
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-user\">${nexell_pool_user}</div>"
		echo "<div id=\"cbip-table-1-user\"></div>"
		echo "</td>"
		
		nexell_pool_status=${nexell_tmp#*Status=}
		nexell_pool_status=${nexell_pool_status%%,Priority=*}
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-status\">${nexell_pool_status}</div>"
		echo "<div id=\"cbip-table-1-status\"></div>"
		echo "</td>"

                nexell_pool_diff=${nexell_tmp#*Work Difficulty=}
		nexell_pool_diff=${nexell_pool_diff%%,Has Stratum=*}
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-diff\">${nexell_pool_diff}</div>"
		echo "<div id=\"cbip-table-1-diff\"></div>"
		echo "</td>"

                nexell_pool_getworks=${nexell_tmp#*Getworks=}
		nexell_pool_getworks=${nexell_pool_getworks%%,Accepted=*}
#2014-10-16 ##################################################################################### 
nexell_pool_getworks_split=$(echo $nexell_pool_getworks| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let nexell_pool_getworks_total+=$nexell_pool_getworks
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-getworks\">${nexell_pool_getworks_split}</div>"
		echo "<div id=\"cbip-table-1-getworks\"></div>"
		echo "</td>"


		
		nexell_pool_priority=${nexell_tmp#*Priority=}
		nexell_pool_priority=${nexell_pool_priority%%,Quota=*}
#2014-10-16 ##################################################################################### 
nexell_pool_priority_split=$(echo $nexell_pool_priority| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-priority\">${nexell_pool_priority_split}</div>"
		echo "<div id=\"cbip-table-1-priority\"></div>"
		echo "</td>"

		nexell_pool_accepted=${nexell_tmp#*Accepted=}
		nexell_pool_accepted=${nexell_pool_accepted%%,Rejected=*}
#2014-10-16 ##################################################################################### 
nexell_pool_accepted_split=$(echo $nexell_pool_accepted| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let nexell_pool_accepted_total+=$nexell_pool_accepted
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-accepted\">${nexell_pool_accepted_split}</div>"
		echo "<div id=\"cbip-table-1-accepted\"></div>"
		echo "</td>"
		

		nexell_pool_diff1=${nexell_tmp#*Diff1 Shares=}
		nexell_pool_diff1=${nexell_pool_diff1%%,Proxy Type=*}
#2014-10-16##################################################################################### 
nexell_pool_diff1_split=$(echo $nexell_pool_diff1| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let nexell_pool_diff1_total+=$nexell_pool_diff1
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-diff1shares\">${nexell_pool_diff1_split}</div>"
		echo "<div id=\"cbip-table-1-diff1shares\"></div>"
		echo "</td>"
		
		nexell_pool_diffa=${nexell_tmp#*Difficulty Accepted=}
		nexell_pool_diffa=${nexell_pool_diffa%%,Difficulty Rejected=*}
		nexell_pool_diffa=${nexell_pool_diffa%%.*}
#2014-10-16 ##################################################################################### 
nexell_pool_diffa_split=$(echo $nexell_pool_diffa| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let nexell_pool_diffa_total+=$nexell_pool_diffa
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-diffaccepted\">${nexell_pool_diffa_split}</div>"
		echo "<div id=\"cbip-table-1-diffaccepted\"></div>"
		echo "</td>"
		
		nexell_pool_diffr=${nexell_tmp#*Difficulty Rejected=}
		nexell_pool_diffr=${nexell_pool_diffr%%,Difficulty Stale=*}
		nexell_pool_diffr=${nexell_pool_diffr%%.*}
#2014-10-16 ##################################################################################### 
nexell_pool_diffr_split=$(echo $nexell_pool_diffr| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let nexell_pool_diffr_total+=$nexell_pool_diffr
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-diffrejected\">${nexell_pool_diffr_split}</div>"
		echo "<div id=\"cbip-table-1-diffrejected\"></div>"
		echo "</td>"
		
		nexell_pool_diffs=${nexell_tmp#*Difficulty Stale=}
		nexell_pool_diffs=${nexell_pool_diffs%%,Last Share Difficulty=*}
		nexell_pool_diffs=${nexell_pool_diffs%%.*}
#2014-10-16 ##################################################################################### 
nexell_pool_diffs_split=$(echo $nexell_pool_diffs| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let nexell_pool_diffs_total+=$nexell_pool_diffs
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-diffstale\">${nexell_pool_diffs_split}</div>"
		echo "<div id=\"cbip-table-1-diffstale\"></div>"
		echo "</td>"		
		
		
		nexell_pool_rejected=${nexell_tmp#*Rejected=}
		nexell_pool_rejected=${nexell_pool_rejected%%,Works=*}
#2014-10-16##################################################################################### 
nexell_pool_rejected_split=$(echo $nexell_pool_rejected| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let nexell_pool_rejected_total+=$nexell_pool_rejected
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-rejected\">${nexell_pool_rejected_split}</div>"
		echo "<div id=\"cbip-table-1-rejected\"></div>"
		echo "</td>"
		
		nexell_pool_discarded=${nexell_tmp#*Discarded=}
		nexell_pool_discarded=${nexell_pool_discarded%%,Stale=*}
#2014-10-16##################################################################################### 
nexell_pool_discarded_split=$(echo $nexell_pool_discarded| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		let nexell_pool_discarded_total+=$nexell_pool_discarded
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-discarded\">${nexell_pool_discarded_split}</div>"
		echo "<div id=\"cbip-table-1-discarded\"></div>"
		echo "</td>"
		
		nexell_pool_stale=${nexell_tmp#*Stale=}
		nexell_pool_stale=${nexell_pool_stale%%,Get Failures=*}
		let nexell_pool_stale_total+=$nexell_pool_stale
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-stale\">${nexell_pool_stale}</div>"
		echo "<div id=\"cbip-table-1-stale\"></div>"
		echo "</td>"
				
		
		nexell_pool_lsdiff=${nexell_tmp#*Last Share Difficulty=}
		nexell_pool_lsdiff=${nexell_pool_lsdiff%%,Has Stratum=*}
		nexell_pool_lsdiff=${nexell_pool_lsdiff%%.*}
#2014-10-16##################################################################################### 
nexell_pool_lsdiff_split=$(echo $nexell_pool_lsdiff| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-lastsharedifficulty\">${nexell_pool_lsdiff_split}</div>"
		echo "<div id=\"cbip-table-1-lastsharedifficulty\"></div>"
		echo "</td>"
		
		nexell_pool_lstime=${nexell_tmp#*Last Share Time=}
		nexell_pool_lstime=${nexell_pool_lstime%%,Diff1 Shares=*}
if [ "$nexell_pool_lstime" = 0 ];
then
 nexell_pool_lstime=Never
fi

		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-lastsharetime\">${nexell_pool_lstime}</div>"
		echo "<div id=\"cbip-table-1-lastsharetime\"></div>"
		echo "</td>"
	done
	
	nexell_pool_diff1_ratio=0
	nexell_pool_diffa_ratio=0
	
	if [ "$nexell_pool_diff1_total" -ne "0" ]; then
		nexell_pool_diff1_ratio=`awk 'BEGIN{printf "%.4f%\n",'$nexell_hw'/'$nexell_pool_diff1_total'*100}'`
	fi
	
	if [ "$nexell_pool_diffa_total" -ne "0" ]; then
		nexell_pool_diffa_ratio=`awk 'BEGIN{printf "%.4f%\n",'$nexell_hw'/'$nexell_pool_diffa_total'*100}'`
	fi
	
	echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-pool\">total</div>"
	echo "<div id=\"cbip-table-1-pool\"></div>"
	echo "</td>"
		
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-url\"></div>"
	echo "<div id=\"cbip-table-1-url\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-user\"></div>"
	echo "<div id=\"cbip-table-1-user\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-status\"></div>"
	echo "<div id=\"cbip-table-1-status\"></div>"
	echo "</td>"

        echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diff\"></div>"
	echo "<div id=\"cbip-table-1-diff\"></div>"
	echo "</td>"
	
#2014-10-16##################################################################################### 
nexell_pool_getworks_total_split=$(echo $nexell_pool_getworks_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-getworks\">${nexell_pool_getworks_total_split}</div>"
	echo "<div id=\"cbip-table-1-getworks\"></div>"
	echo "</td>"

	
        echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-priority\"></div>"
	echo "<div id=\"cbip-table-1-priority\"></div>"
	echo "</td>"
	
#2014-10-16##################################################################################### 
nexell_pool_accepted_total_split=$(echo $nexell_pool_accepted_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-accepted\">${nexell_pool_accepted_total_split}</div>"
	echo "<div id=\"cbip-table-1-accepted\"></div>"
	echo "</td>"
	
#2014-10-16##################################################################################### 
nexell_pool_diff1_total_split=$(echo $nexell_pool_diff1_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
        echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diff1shares\">${nexell_pool_diff1_total_split}</div>"
	echo "<div id=\"cbip-table-1-diff1shares\"></div>"
	echo "</td>"

#2014-10-16##################################################################################### 
nexell_pool_diffa_total_split=$(echo $nexell_pool_diffa_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diffaccepted\">${nexell_pool_diffa_total_split}</div>"
	echo "<div id=\"cbip-table-1-diffaccepted\"></div>"
	echo "</td>"
	
#2014-10-16##################################################################################### 
nexell_pool_diffr_total_split=$(echo $nexell_pool_diffr_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diffrejected\">${nexell_pool_diffr_total_split}</div>"
	echo "<div id=\"cbip-table-1-diffrejected\"></div>"
	echo "</td>"
	
#2014-10-16##################################################################################### 
nexell_pool_diffs_total_split=$(echo $nexell_pool_diffs_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diffstale\">${nexell_pool_diffs_total_split}</div>"
	echo "<div id=\"cbip-table-1-diffstale\"></div>"
	echo "</td>"

#2014-10-16##################################################################################### 
nexell_pool_rejected_total_split=$(echo $nexell_pool_rejected_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-rejected\">${nexell_pool_rejected_total_split}</div>"
	echo "<div id=\"cbip-table-1-rejected\"></div>"
	echo "</td>"
	
#2014-10-16##################################################################################### 
nexell_pool_discarded_total_split=$(echo $nexell_pool_discarded_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
############################################################################################### 
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-discarded\">${nexell_pool_discarded_total_split}</div>"
	echo "<div id=\"cbip-table-1-discarded\"></div>"
	echo "</td>"

#2014-10-16##################################################################################### 
nexell_pool_stale_total_split=$(echo $nexell_pool_stale_total| sed -r ':1;s/(.*[0-9])([0-9]{3})/\1,\2/;t1')
###############################################################################################	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-stale\">${nexell_pool_stale_total_split}</div>"
	echo "<div id=\"cbip-table-1-stale\"></div>"
	echo "</td>"
	
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-lastsharedifficulty\"></div>"
	echo "<div id=\"cbip-table-1-lastsharedifficulty\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-lastsharetime\"></div>"
	echo "<div id=\"cbip-table-1-lastsharetime\"></div>"
	echo "</td>"
	
	echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-pool\">HW</div>"
	echo "<div id=\"cbip-table-1-pool\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-url\">${nexell_hw}</div>"
	echo "<div id=\"cbip-table-1-url\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-user\"></div>"
	echo "<div id=\"cbip-table-1-user\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-status\"></div>"
	echo "<div id=\"cbip-table-1-status\"></div>"
	echo "</td>"
	
echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diff\"></div>"
	echo "<div id=\"cbip-table-1-diff\"></div>"
	echo "</td>"
	
echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-getworks\"></div>"
	echo "<div id=\"cbip-table-1-getworks\"></div>"
	echo "</td>"

	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-priority\"></div>"
	echo "<div id=\"cbip-table-1-priority\"></div>"
	echo "</td>"		
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-accepted\"></div>"
	echo "<div id=\"cbip-table-1-accepted\"></div>"
	echo "</td>"

echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diff1shares\">${nexell_pool_diff1_ratio}</div>"
	echo "<div id=\"cbip-table-1-diff1shares\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diffaccepted\">${nexell_pool_diffa_ratio}</div>"
	echo "<div id=\"cbip-table-1-diffaccepted\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diffrejected\"></div>"
	echo "<div id=\"cbip-table-1-diffrejected\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-diffstale\"></div>"
	echo "<div id=\"cbip-table-1-diffstale\"></div>"
	echo "</td>"


	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-rejected\"></div>"
	echo "<div id=\"cbip-table-1-rejected\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-discarded\"></div>"
	echo "<div id=\"cbip-table-1-discarded\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-stale\"></div>"
	echo "<div id=\"cbip-table-1-stale\"></div>"
	echo "</td>"	
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-lastsharedifficulty\"></div>"
	echo "<div id=\"cbip-table-1-lastsharedifficulty\"></div>"
	echo "</td>"
	
	echo "<td class=\"cbi-value-field\">"
	echo "<div id=\"cbi-table-1-lastsharetime\"></div>"
	echo "<div id=\"cbip-table-1-lastsharetime\"></div>"
	echo "</td>"
fi

cat <<-EOD
						</table>
					</div>
				</fieldset>
				<!-- /tblsection -->
				<!-- tblsection -->
				<fieldset class="cbi-section" id="cbi-table-table">
					<legend>BitMiner</legend>
					<div class="cbi-section-descr"></div>
					<div class="cbi-section-node">
						<table id="nexell_devs" class="cbi-section-table">
							<tr class="cbi-section-table-titles">
								<th class="cbi-section-table-cell">Chain#</th>
								<th class="cbi-section-table-cell">ASIC#</th>
								<th class="cbi-section-table-cell">Volts</th>
								<th class="cbi-section-table-cell">Highest Temperature</th>
								<th class="cbi-section-table-cell">Hottest ASIC</th>
							</tr>
							<tr class="cbi-section-table-descr">
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
								<th class="cbi-section-table-cell"></th>
							</tr>
EOD

nexell_tmp=`cgminer-api -o stats`
if [ "${nexell_tmp}" != "Socket connect failed: Connection refused" ]; then
	
	while :;
	do
		nexell_tmp=${nexell_tmp#*chain_id=}
		nexell_len=${#nexell_tmp}
	
		if [ ${nexell_len} -eq ${nexell_last_len} ]; then
			break
		fi
		nexell_last_len=${nexell_len}
		
		echo "<tr class=\"cbi-section-table-row cbi-rowstyle-1\" id=\"cbi-table-1\">"
		
		nexell_chain_id=
		nexell_asics=
		nexell_volts=
		nexell_hi_temp=
		nexell_hot_chip=
		
		nexell_chain_id=${nexell_tmp%%,asic_count=*}
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-pool\">${nexell_chain_id}</div>"
		echo "<div id=\"cbip-table-1-pool\"></div>"
		echo "</td>"
		
		nexell_asics=${nexell_tmp#*asic_count=}
		nexell_asics=${nexell_asics%%,volt=*}
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-url\">${nexell_asics}</div>"
		echo "<div id=\"cbip-table-1-url\"></div>"
		echo "</td>"
		
		nexell_volts=${nexell_tmp#*volt=}
		nexell_volts=${nexell_volts%%,hi_temp=*}
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-url\">${nexell_volts}</div>"
		echo "<div id=\"cbip-table-1-url\"></div>"
		echo "</td>"
		
		nexell_hi_temp=${nexell_tmp#*hi_temp=}
		nexell_hi_temp=${nexell_hi_temp%%,hot_chip=*}
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-url\">${nexell_hi_temp}</div>"
		echo "<div id=\"cbip-table-1-url\"></div>"
		echo "</td>"
		
		nexell_hot_chip=${nexell_tmp#*hot_chip=}
		nexell_hot_chip=${nexell_hot_chip%%,status=*}
		echo "<td class=\"cbi-value-field\">"
		echo "<div id=\"cbi-table-1-url\">${nexell_hot_chip}</div>"
		echo "<div id=\"cbip-table-1-url\"></div>"
		echo "</td>"
	done
fi

cat <<-EOF
						</table>
					</div>
				</fieldset>
				<!-- /tblsection -->
				<br />
			</div>
			<div class="clear"></div>
		</div>
	</div>
	<div class="clear"></div>
	<div style="text-align: center; bottom: 0; left: 0; height: 1.5em; font-size: 80%; margin: 0; padding: 5px 0px 2px 8px; background-color: #918ca0; width: 100%;">
		<font style="color:#fff;">Copyright &copy; 2009-2018, Nexell Technologies</font>
	</div>
</body>
</html>
EOT
