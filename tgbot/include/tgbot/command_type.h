#pragma once

namespace ymsummorizer::tgbot {

  /// @brief Тип команды
  /// Команды обычного пользователя:
  /// * start,
  /// * user_token_add,
  /// * user_token_erase,
  /// * group_list,
  /// * group_leave,
  /// * group_playslit_list,
  /// * group_playslit_view
  /// Команды администратора группы:
  /// * user_view,
  /// * group_user_add,
  /// * group_user_remove,
  /// * group_playslit_add,
  /// * group_playslit_remove
  /// Команды обычного администратора:
  /// * group_create,
  /// * group_delete,
  /// Команды суперадминистратора:
  /// * access_edit
  enum class command_type {
    /// @brief Добавление пользователя в БД
    /// (берём его имя, фамилию, логин из сообщения в ТГ и запоминаем)
    start,
    /// @brief Добавление токена пользователя в БД.
    /// Аргумент - токен Яндекса. Записываем логин Яндекса в БД
    user_token_add,
    /// @brief Удаление токена пользователя из БД.
    /// Должно выкинуть его из групп?
    user_token_erase,
    /// @brief Изменение токена пользователя в БД.
    /// Аргумент - команда: переписать, удалить.
    /// Аргумент - токен Яндекса, если переписать.
    // TODO: token_edit,
    /// @brief Посмотреть информацию о пользователе.
    /// Аргумент - логин ТГ.
    user_view,

    /// @brief Просмотреть список групп для пользователя.
    group_list,
    /// @brief Добавление новой группы пользователей
    /// Аргумент - название группы.
    group_create,
    /// @brief Удаление старой группы пользователей
    /// Аргумент - название группы.
    group_delete,

    /// @brief Добавление пользователя в группу
    /// Аргумент - название группы.
    /// Аргумент - логин ТГ.
    group_user_add,
    /// @brief Удаление пользователя из группы
    /// Аргумент - название группы.
    /// Аргумент - логин ТГ.
    group_user_remove,
    /// @brief Покинуть группу пользователем.
    /// Аргумент - название группы.
    group_leave,

    /// @brief Просмотреть список плейлистов в группе
    /// Аргумент - название группы.
    group_playslit_list,
    /// @brief Просмотреть плейлист
    /// Аргумент - название группы.
    /// Аргумент - название плейлиста.
    group_playslit_view,
    /// @brief Добавить плейлист в группу
    /// Аргумент - название группы.
    /// Аргумент - название плейлиста.
    group_playslit_add,
    /// @brief Удалить плейлист из группы
    /// Аргумент - название группы.
    /// Аргумент - название плейлиста.
    group_playslit_remove,

    // TODO(Arseny802): добавить команду для редактирования прав доступа пользователя из телеграма
    /// @brief Редактирование прав доступа пользователя.
    /// Аргумент - логин ТГ.
    /// Аргумент - маска прав доступа.
    // access_edit
  };
} // namespace ymsummorizer::tgbot