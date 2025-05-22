(function ($) {
  Drupal.behaviors.project = {
    attach: function (context, settings) {
      const menuHideWidth = settings.theme.nav_mobile_hide_width;

      // --- Страница регистраций ----------------------------------------------
      if ($(window).width() >= menuHideWidth) {
        // для пустых таблиц на десктопе показать сообщение
        $('.table-li-responsive').each(function (i, table) {
          var isTableEmpty = true;
          $(table).find('.table-row').each(function (i, li) {
            if ($(li).height()) isTableEmpty = false;
          });
          if (isTableEmpty) $(table).find(".table-empty").show();
        });

        //  установить текущую высоту таблиц как их минимальную,
        //  чтобы экран не прыгал на небольших выборках
        $('.table-li-responsive').each(function () {
          $(this).css("min-height", $(this).height());
        });
      }

        $(context).find(".table-row").on('click', function() {
          let clicked = $(this).hasClass('clicked');

          // выключаем клик по всем строкам
          let $parent = $(this).closest('.tables-set');
          $parent.find(".table-base .clicked").each((i, el) => {
            turnOffSiblings(el, 'clicked');
          });
          $parent.find(".table-row .col-select").text("");
          if ($(window).width() < menuHideWidth) $parent.find('.table-base li:not(.table-header)').removeClass('not-clicked');

          // если клик был по отключенной строке, включаем её
          if (!clicked) {
            turnOnSiblings(this, 'clicked');
            $(this).addClass('clicked').find(".col-select").text("v");
            if ($(window).width() < menuHideWidth) $parent.find('.table-base li:not(.table-header):not(.clicked)').addClass('not-clicked');
          }
        });

      function turnOnSiblings(el, cl) {
        let $parent = $(el).closest('.tables-set');
        let $reg_id = $(el).data('id');
        if (!$reg_id) return;
        $(el).addClass(cl);
        $parent.find('.table-siblings li:not(.table-header)').each(function() {
          let $base_reg_id = $(this).data('base-id');
          if ($base_reg_id == $reg_id) {
            $(this).addClass(cl);
          } else {
            $(this).addClass('not-' + cl);
          }
        });
      }
      function turnOffSiblings(el, cl) {
        let $parent = $(el).closest('.tables-set');
        let $reg_id = $(el).data('id');
        if (!$reg_id) return;
        $(el).removeClass(cl);
        $parent.find('.table-siblings li:not(.table-header)').each(function() {
          let $base_reg_id = $(this).data('base-id');
          if ($base_reg_id == $reg_id) {
            $(this).removeClass(cl);
          } else {
            $(this).removeClass('not-' + cl);
          }
        });

      }

      // --  Финансы ---------------------------------------- ----------------------------------------------------------
      // -- collapsible ячейки на странице
      $(context).find(".table-row-toggle").once( function () {
        $(this).on("click", (el) => {
          const target = $(el.target).data("target");
          if ($("." + target).hasClass('show')) {
            $("." + target).slideUp(200).removeClass('show');
            $(el.target).text('+');
          } else {
            $("." + target).slideDown(200).addClass('show');
            $(el.target).text('-');
          }
        });
      });

      // -- установка высоты, чтобы страница не скакала
      // низ обёртки должен быть не выше низа экрана
      $(".table-row-toggle").on("click", (el) => {
        let wrOffset = $("#table-wrapper").offset();
        let wrHeight = $("#table-wrapper").height();
        let docScroll = $(document).scrollTop();
        let wrBottom = wrOffset.top + wrHeight - docScroll;
        let brBottom = $(window).height();
        // подвал пусть будет виден
        let docHeight = $(document).height();
        let wrPosBottom = docHeight - wrOffset.top - wrHeight;

        $("#table-wrapper").css("min-height", wrHeight + brBottom - wrBottom - wrPosBottom+14);
      });

    }
  };
})(jQuery);
