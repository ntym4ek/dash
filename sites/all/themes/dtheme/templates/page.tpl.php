<div class="page-wrapper">

  <input type="checkbox" id="navigation" />
  <label class="mobile-menu-label" for="navigation">
      <div class="label">
        <div class="icon"></div>
      </div>
  </label>

  <div class="navigation">
    <div class="logo">
      <img src="<?php print $logo; ?>" />
      <span>Статистика</span>
    </div>
    <div class="mobile-menu">
      <?php if ($primary_nav): print $primary_nav; endif; ?>
      <?php if ($secondary_nav): print $secondary_nav; endif; ?>
    </div>
  </div>

  <div class="page">
    <header class="header">
      <div class="container">
        <div class="row middle-xs">
          <div class="col-xs-12 col-md-2 col-lg-1 branding">
            <a href="<?php print $front_page ?>">
              <img src="<?php print $logo ?>" />
            </a>
          </div>

          <div class="col-xs-12 col-md-10 col-lg-11 hide-xs show-md menu">
            <div class="row">
              <div class="col-xs-6 primary-menu"><?php if ($primary_nav): print $primary_nav; endif; ?></div>
              <div class="col-xs-6 secondary-menu"><?php if ($secondary_nav): print $secondary_nav; endif; ?></div>
            </div>
          </div>
        </div>
      </div>
    </header>

    <div class="main">
      <div class="container">
        <div class="content">
          <?php print render($title_prefix); ?>
          <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
          <?php print render($title_suffix); ?>

          <?php if (isset($tabs)): ?><?php print render($tabs); ?><?php endif; ?>
          <?php print $messages; ?>
          <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>


          <?php print render($page['content']); ?>
        </div>
      </div>
    </div>

    <div class="footer">
      <div class="container">
        <div class="row middle-xs">
          <div class="col-xs-6 branding"><img class="logo" src="<?php print $logo; ?>" /></div>
          <div class="col-xs-6 rights">© <?php print date('Y', time()); ?> KCCC GROUP</div>
        </div>
      </div>
    </div>
  </div>

</div>

