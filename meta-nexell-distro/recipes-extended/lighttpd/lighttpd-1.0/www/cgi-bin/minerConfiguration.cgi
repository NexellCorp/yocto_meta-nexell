#!/bin/sh -e
#set -x

create_default_conf_file()
{
(
cat <<'EOF'
{
"pools" : [
{
"url" : "stratum+tcp://sg.stratum.slushpool.com:3333",
"user" : "maekdal.bcworker1",
"pass" : "123"
},
{
"url" : "stratum+tcp://sg.stratum.slushpool.com:3333",
"user" : "maekdal.bcworker1",
"pass" : "123"
},
{
"url" : "stratum+tcp://sg.stratum.slushpool.com:3333",
"user" : "maekdal.bcworker1",
"pass" : "123"
}
]
,
"api-listen" : true,
"api-network" : true,
"api-allow" : "W:0/0",
"miner-freq": "18:218.75:1106",
"miner-voltage": "0725"
}

EOF
) > /config/cgminer.conf
}

if [ ! -f /config/cgminer.conf ] ; then
    if [ -f /config/cgminer.conf.factory ] ; then
		cp /config/cgminer.conf.factory /config/cgminer.conf
    else
		create_default_conf_file
		cp /config/cgminer.conf /config/cgminer.conf.factory
    fi
fi

nexell_result=`cat /config/cgminer.conf`

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
<script>
EOH

echo "nexell_data = ${nexell_result};"

cat <<EOT
function f_get_miner_conf() {
	try
	{
		for(var i = 0; i < nexell_data.pools.length; i++) {
			switch(i) {
			case 0:
				jQuery("#nexell_pool1url").val(nexell_data.pools[i].url);
				jQuery("#nexell_pool1user").val(nexell_data.pools[i].user);
				jQuery("#nexell_pool1pw").val(nexell_data.pools[i].pass);
				break;
			case 1:
				jQuery("#nexell_pool2url").val(nexell_data.pools[i].url);
				jQuery("#nexell_pool2user").val(nexell_data.pools[i].user);
				jQuery("#nexell_pool2pw").val(nexell_data.pools[i].pass);
				break;
			case 2:
				jQuery("#nexell_pool3url").val(nexell_data.pools[i].url);
				jQuery("#nexell_pool3user").val(nexell_data.pools[i].user);
				jQuery("#nexell_pool3pw").val(nexell_data.pools[i].pass);
				break;
			}
		}
		if(nexell_data["bitmain-nobeeper"]) {
			document.getElementById("nexell_beeper").checked = false;
		} else {
			document.getElementById("nexell_beeper").checked = true;
		}
		if(nexell_data["bitmain-notempoverctrl"]) {
			document.getElementById("nexell_tempoverctrl").checked = false;
		} else {
			document.getElementById("nexell_tempoverctrl").checked = true;
		}
	}
	catch(err)
	{
		alert('Invalid Miner configuration file. Edit manually or reset to default.');
	}
}
function f_submit_miner_conf() {
	_nexell_freq = "18:218.75:1106";
	_nexell_voltage = "0725";
	try
	{
		_nexell_freq = nexell_data["miner-freq"];
		_nexell_voltage = nexell_data["miner-voltage"];
	}
	catch(err)
	{
		alert('Invalid Miner configuration file. Edit manually or reset to default.');
	}
	
	_nexell_pool1url = jQuery("#nexell_pool1url").val();
	_nexell_pool1user = jQuery("#nexell_pool1user").val();
	_nexell_pool1pw = jQuery("#nexell_pool1pw").val();
	_nexell_pool2url = jQuery("#nexell_pool2url").val();
	_nexell_pool2user = jQuery("#nexell_pool2user").val();
	_nexell_pool2pw = jQuery("#nexell_pool2pw").val();
	_nexell_pool3url = jQuery("#nexell_pool3url").val();
	_nexell_pool3user = jQuery("#nexell_pool3user").val();
	_nexell_pool3pw = jQuery("#nexell_pool3pw").val();
	_nexell_nobeeper = "false";
	_nexell_notempoverctrl = "false";
	
	if(document.getElementById("nexell_beeper").checked) {
		_nexell_nobeeper = "false";
	} else {
		_nexell_nobeeper = "true";
	}
	if(document.getElementById("nexell_tempoverctrl").checked) {
		_nexell_notempoverctrl = "false";
	} else {
		_nexell_notempoverctrl = "true";
	}
	
	jQuery("#cbi_apply_cgminer_fieldset").show();
	
	jQuery.ajax({
		url: '/cgi-bin/set_miner_conf.cgi',
		type: 'POST',
		dataType: 'json',
		timeout: 30000,
		cache: false,
		data: {_nexell_pool1url:_nexell_pool1url, _nexell_pool1user:_nexell_pool1user, _nexell_pool1pw:_nexell_pool1pw,_nexell_pool2url:_nexell_pool2url, _nexell_pool2user:_nexell_pool2user, _nexell_pool2pw:_nexell_pool2pw,_nexell_pool3url:_nexell_pool3url, _nexell_pool3user:_nexell_pool3user, _nexell_pool3pw:_nexell_pool3pw, _nexell_nobeeper:_nexell_nobeeper, _nexell_notempoverctrl:_nexell_notempoverctrl, _nexell_freq:_nexell_freq, _nexell_voltage:_nexell_voltage},
		success: function(data) {
			window.location.reload();
		},
		error: function() {
			window.location.reload();
		}
	});
}

jQuery(document).ready(function() {
	f_get_miner_conf();
});
</script>
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
					<li class="tabmenu-item-system active"><a href="/cgi-bin/minerConfiguration.cgi">Miner Configuration</a></li>
					<li class="tabmenu-item-network"><a href="/cgi-bin/minerStatus.cgi">Miner Status</a></li>
					<li class="tabmenu-item-system"><a href="/network.html">Network</a></li>
				</ul>
				<br style="clear: both" />
				<div class="tabmenu2">
					<ul class="tabmenu l2">
						<li class="tabmenu-item-system active"><a href="/cgi-bin/minerConfiguration.cgi">General Settings</a></li>
						<li class="tabmenu-item-system"><a href="/cgi-bin/minerAdvanced.cgi">Advanced Settings</a></li>
					</ul>
					<br style="clear: both" />
				</div>
			</div>
		</div>
		<div id="maincontent">
			<noscript>
				<div class="errorbox">
					<strong>Java Script required!</strong><br /> You must enable Java Script in your browser or LuCI will not work properly.
				</div>
			</noscript>
			<h2 style="padding-bottom:10px;"><a id="content" name="content">Miner General Configuration</a></h2>
			<div class="cbi-map" id="cbi-cgminer">
				<fieldset class="cbi-section" id="cbi_msg_cgminer_fieldset" style="display:none">
					<span id="cbi_msg_cgminer" style="color:red;"></span>
				</fieldset>
				<fieldset class="cbi-section" id="cbi_apply_cgminer_fieldset" style="display:none">
					<img src="/resources/icons/loading.gif" alt="Loading" style="vertical-align:middle" />
					<span id="cbi-apply-cgminer-status">Waiting for changes to be applied...</span>
				</fieldset>
				<fieldset class="cbi-section" id="cbi-cgminer-cgminer">
					<div class="cbi-section-descr"></div>
					<fieldset class="cbi-section" id="cbi-cgminer-default">
						<legend>Pool 1</legend>
						<div class="cbi-value" id="cbi-cgminer-default-pool1url">
							<label class="cbi-value-title" for="cbid.cgminer.default.pool1url">URL</label>
							<div class="cbi-value-field">
								<input type="text" class="cbi-input-text" name="cbid.cgminer.default.pool1url" id="nexell_pool1url" value="" />
							</div>
						</div>
						<div class="cbi-value" id="cbi-cgminer-default-pool1user">
							<label class="cbi-value-title" for="cbid.cgminer.default.pool1user">Worker</label>
							<div class="cbi-value-field">
								<input type="text" class="cbi-input-text" name="cbid.cgminer.default.pool1user" id="nexell_pool1user" value="" />
							</div>
						</div>
						<div class="cbi-value" id="cbi-cgminer-default-pool1pw">
							<label class="cbi-value-title" for="cbid.cgminer.default.pool1pw">Password</label>
							<div class="cbi-value-field">
								<input type="text" class="cbi-input-text" name="cbid.cgminer.default.pool1pw" id="nexell_pool1pw" value="" />
							</div>
						</div>
					</fieldset>
					<fieldset class="cbi-section" id="cbi-cgminer-default">
						<legend>Pool 2</legend>
						<div class="cbi-value" id="cbi-cgminer-default-pool2url">
							<label class="cbi-value-title" for="cbid.cgminer.default.pool2url">URL</label>
							<div class="cbi-value-field">
								<input type="text" class="cbi-input-text" name="cbid.cgminer.default.pool2url" id="nexell_pool2url" value="" />
							</div>
						</div>
						<div class="cbi-value" id="cbi-cgminer-default-pool2user">
							<label class="cbi-value-title" for="cbid.cgminer.default.pool2user">Worker</label>
							<div class="cbi-value-field">
								<input type="text" class="cbi-input-text" name="cbid.cgminer.default.pool2user" id="nexell_pool2user" value="" />
							</div>
						</div>
						<div class="cbi-value" id="cbi-cgminer-default-pool2pw">
							<label class="cbi-value-title" for="cbid.cgminer.default.pool2pw">Password</label>
							<div class="cbi-value-field">
								<input type="text" class="cbi-input-text" name="cbid.cgminer.default.pool2pw" id="nexell_pool2pw" value="" />
							</div>
						</div>
					</fieldset>
					<fieldset class="cbi-section" id="cbi-cgminer-default">
						<legend>Pool 3</legend>
						<div class="cbi-value" id="cbi-cgminer-default-pool3url">
							<label class="cbi-value-title" for="cbid.cgminer.default.pool3url">URL</label>
							<div class="cbi-value-field">
								<input type="text" class="cbi-input-text" name="cbid.cgminer.default.pool3url" id="nexell_pool3url" value="" />
							</div>
						</div>
						<div class="cbi-value" id="cbi-cgminer-default-pool3user">
							<label class="cbi-value-title" for="cbid.cgminer.default.pool3user">Worker</label>
							<div class="cbi-value-field">
								<input type="text" class="cbi-input-text" name="cbid.cgminer.default.pool3user" id="nexell_pool3user" value="" />
							</div>
						</div>
						<div class="cbi-value cbi-value-last"
							id="cbi-cgminer-default-pool3pw">
							<label class="cbi-value-title" for="cbid.cgminer.default.pool3pw">Password</label>
							<div class="cbi-value-field">
								<input type="text" class="cbi-input-text" name="cbid.cgminer.default.pool3pw" id="nexell_pool3pw" value="" />
							</div>
						</div>
					</fieldset>
					<fieldset class="cbi-section" id="cbi-cgminer-default">
						<legend>Setup</legend>
						<div class="cbi-value" id="cbi-cgminer-default-pool3url">
							<label class="cbi-value-title" for="keep">Beeper ringing</label>
							<div class="cbi-value-field">
								<input type="checkbox" name="nexell_beeper" id="nexell_beeper" checked />
							</div>
						</div>
						<div class="cbi-value" id="cbi-cgminer-default-pool3user">
							<label class="cbi-value-title" for="keep">Stop running when temprerature is over 80℃</label>
							<div class="cbi-value-field">
								<input type="checkbox" name="nexell_tempoverctrl" id="nexell_tempoverctrl" checked />
							</div>
						</div>
					</fieldset>
					<br />
				</fieldset>
				<br />
			</div>
			<div class="cbi-page-actions">
				<input class="cbi-button cbi-button-save right" type="button" onclick="f_submit_miner_conf();" value="Save&Apply" />
				<input class="cbi-button cbi-button-reset right" type="button" onclick="f_get_miner_conf();" value="Reset" />
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
