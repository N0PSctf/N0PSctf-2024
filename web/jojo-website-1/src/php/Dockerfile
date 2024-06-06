FROM php:8.2-apache

RUN docker-php-ext-install mysqli
RUN pecl install redis && docker-php-ext-enable redis
RUN docker-php-ext-install mysqli
COPY --chown=www-data:www-data ./public_html/ /var/www/html
RUN chown -R www-data:www-data /var/www/
COPY ./jojo_website.conf /etc/apache2/sites-available/jojo_website.conf
RUN a2dissite 000-default && a2ensite jojo_website && service apache2 restart

EXPOSE 80