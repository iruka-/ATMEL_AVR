<?php
/**
 * Monobook for PukiWiki
 * by lunt
 *
 * Based on original Monobook's MediaWiki skin.
 *
 * @author     lunt
 * @license    http://www.gnu.org/licenses/gpl.html GPL 2
 * @version    $Id: monobook.skin.php 168 2007-03-29 15:59:41Z lunt $
 */

// ------------------------------------------------------------
// Setting

// Set image
$logo = 'title.jpg'; //'pukiwiki.png';
$favicon = '';

// Select navigation tabs as follows:
/* add, attachlist, attachlistall, backup, copy, diff, edit, filelist, freeze, help, list, new, rdf,
   recent, refer, related, reload, rename, rss, rss10, rss20, search, source, top, trackback, upload, yetlist */
$tabs = array('edit', 'diff');

// Set wikinote plugin
$wikinote_prefix = 'note/';
$wikinote_except = '^$';
$wikinote_noauto = true;

// Enable paraedit plugin
$enable_paraedit = false;

// Enable access counter in footer
$enable_footer_counter = false;

// ------------------------------------------------------------
// Code

if (! defined('UI_LANG')) die('UI_LANG is not set');
if (! isset($_LANG)) die('$_LANG is not set');
if (! defined('PKWK_READONLY')) die('PKWK_READONLY is not set');
if (! exist_plugin('monobook_navigation')) die('monobook_navigation plugin not found');

// Background color
global $rule_page, $whatsdeleted, $interwiki;
$background = '';
if (
	empty($vars['page']) ||
	substr($vars['page'], 0, strlen($wikinote_prefix)) === $wikinote_prefix ||
	in_array($vars['page'], array($help_page, $rule_page, $whatsnew, $whatsdeleted, $interwiki))) {
		$background = ' class="specialbg"';
}

// Navigation tab
$wikinote_ini = array('prefix' => $wikinote_prefix, 'except' => $wikinote_except, 'noauto' => $wikinote_noauto);
$navigation_tab = plugin_monobook_navigation($wikinote_ini, $tabs, $background);

// Title
global $_monobook_navigation_messages;
if (substr($title, 0, strlen($wikinote_prefix)) === $wikinote_prefix) {
	$display_title = $_monobook_navigation_messages['discuss'] . ':' . substr(strstr($title, '/'), 1);
} else {
	$display_title = $title;
}

// paraedit.inc.php
if ($enable_paraedit && exist_plugin('paraedit'))
	$body = _plugin_paraedit_mkeditlink($body);

// Footer
$lastmodified = empty($lastmodified) ? '' : '<li id="lastmod">Last-modified: ' . $lastmodified . '</li>';
$siteadmin = ! empty($modifierlink) && ! empty($modifier) ?
	'<li>Site admin: <a href="' . $modifierlink . '">' . $modifier . '</a></li>' : '';
$footer_counter = $enable_footer_counter && exist_plugin('counter') && do_plugin_inline('counter') ?
	'<li>This page has been accessed ' . do_plugin_inline('counter') . ' times.</li>' : '';

// ------------------------------------------------------------
// Output

// HTTP headers
pkwk_common_headers();
header('Cache-control: no-cache');
header('Pragma: no-cache');
header('Content-Type: text/html; charset=' . CONTENT_CHARSET);

// HTML DTD
if (isset($pkwk_dtd)) {
	$meta_content_type = pkwk_output_dtd($pkwk_dtd);
} else {
	$meta_content_type = pkwk_output_dtd();
}
?>

<head>
 <?php echo $meta_content_type ?>
<?php if ($nofollow || ! $is_read)  { ?> <meta name="robots" content="NOINDEX,NOFOLLOW" /><?php } ?>
<?php if (PKWK_ALLOW_JAVASCRIPT && isset($javascript)) { ?> <meta http-equiv="Content-Script-Type" content="text/javascript" /><?php } ?>
 <title><?php echo $title ?> - <?php echo $page_title ?></title>
<?php if ($favicon) echo ' <link rel="shortcut icon" href="' . IMAGE_DIR . $favicon . '" type="image/x-icon" />' ?>
 <link rel="stylesheet" type="text/css" media="screen" href="<?php echo SKIN_DIR ?>monobook/monobook.css" />
 <link rel="stylesheet" type="text/css" media="print" href="<?php echo SKIN_DIR ?>monobook/monobook.print.css" />
 <link rel="alternate" type="application/rss+xml" title="RSS" href="<?php echo $_LINK['rss'] ?>" />
<?php echo $head_tag ?>
</head>
<body>
<div id="globalWrapper">
 <div id="menubar"><?php exist_plugin_convert('menu') ?><?php echo do_plugin_convert('menu') ?></div>
 <div id="mainColumnWrapper">
  <div id="main-column">
   <?php echo $navigation_tab ?>
   <div id="content"<?php echo $background ?>>
    <h1 class="firstHeading"><?php echo $display_title ?></h1><div id="contentSub"></div>
    <?php echo $body ?><?php echo $notes ?>
   </div>
  </div>
 </div>
 <div style="clear:both;height:1em;"></div>
 <div id="logo"><a href="<?php echo get_script_uri() ?>" style="background-image: url(<?php echo IMAGE_DIR . $logo ?>);"></a></div>
 <div id="personal"><ul><li class="login"><a href="#">ログインまたはアカウント作成</a></li></ul></div>
 <div id="footer">
  <div id="f-officialico">
   <a href="http://pukiwiki.sourceforge.jp/"><img src="<?php echo IMAGE_DIR ?>b_pukiwiki.official.png" alt="PukiWiki-official" /></a>
  </div>
  <div id="f-officialdevico">
   <a href="http://pukiwiki.sourceforge.jp/dev/"><img src="<?php echo IMAGE_DIR ?>b_pukiwiki.dev.png" alt="PukiWiki-dev" /></a>
  </div>
  <div id="f-list">
   <ul><?php echo $lastmodified . $siteadmin . $footer_counter ?>
    <li>convert time: <?php echo $taketime ?> sec</li>
    <li>Powered by PukiWiki</li>
    <li><a href="http://www.luntf.com/">Monobook for PukiWiki</a></li>
   </ul>
  </div>
  <div style="clear:both;"></div>
 </div>
</div>
</body>
</html>
