(function ($) {
  Drupal.behaviors.dtheme = {
    attach: function (context, settings) {
      // --- Десктоп меню ----------------------------------------------
        // убрать реакцию на клик для пунктов с подменю
      $('.menu .expanded > a').on('click', () => {
        return false;
      });

      // --- Свайп меню --------------------------------------------------------
        // если < 768 то выводится боковое меню
        // повесить обработчик свайпа
      if ($(window).width() < 768) {
        $('.page, label.mobile-menu').on("swiped-right", () => {
          $('#navigation').prop('checked', true);
        });
        $('.page, .navigation, label.mobile-menu').on("swiped-left", () => {
          $('#navigation').prop('checked', false);
        });
        $('.page').on("click", () => {
          $('#navigation').prop('checked', false);
        });
      }

      // --- Страница регистраций ----------------------------------------------
      $('.table-base li').on('click', function() {
        let $parent = $(this).closest('.tables-set');
        if ($(this).hasClass('clicked')) {
          turnOffSiblings(this, 'clicked');
          $(this).removeClass('clicked');
          $parent.find('.table-base li:not(.table-header)').removeClass('not-clicked');
        } else {
          turnOnSiblings(this, 'clicked');
          $(this).addClass('clicked');
          $parent.find('.table-base li:not(.table-header):not(.clicked)').addClass('not-clicked');
        }
      });
      $('.table-base li').on('mouseover', function() {
        if (!$(this).hasClass('hovered')) {
          $(this).addClass('hovered');
          turnOnSiblings(this, 'hovered');
        }
      });
      $('.table-base li').on('mouseleave', function() {
        if ($(this).hasClass('hovered')) {
          turnOffSiblings(this, 'hovered');
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

      // -- collapsible ячейки на странице Финансы ----------------------------------------------------------
      $(context).find(".table-row-toggle").once( function () {
        $(this).on("click", (el) => {
          const target = $(el.target).data("target");
          if ($('.' + target).hasClass('show')) {
            $('.' + target).slideUp(200);
            $('.' + target).removeClass('show');
            $(el.target).text('+');
          } else {
            $('.' + target).slideDown(200);
            $('.' + target).addClass('show');
            $(el.target).text('-');
          }
        });
      });



    }
  };
})(jQuery);
